// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Player/ForgePlayer.h"

#include "UnrealNetwork.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"

AForgeAnvil::AForgeAnvil()
{

	bHammerMinigamePlaying = false;
	Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating"));
	Rotator->SetupAttachment(StationMesh);
	Rotator->SetRelativeLocation(StationMesh->RelativeLocation);
	
	CurrentProducingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh3"));
	CurrentProducingItem->SetupAttachment(Rotator);
	CurrentProducingItem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CurrentProducingItem->SetVisibility(true, false);

	Rotator->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	CurrentState = EBladeType::BT_BROADSWORD;
	OutputName = FString("Broad Blade");
	CurrentResource = EBladeMat::BM_NONE;

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundSuccess(TEXT("SoundWave'/Game/Sounds/SoundRourke/SmithingHit_Sound.SmithingHit_Sound'"));
	Success = SoundSuccess.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFail(TEXT("SoundWave'/Game/Sounds/SoundRourke/Error_or_Drop_Sound.Error_or_Drop_Sound'"));
	Failure = SoundFail.Object;

	PotentiallyInteracting = false;
}
void AForgeAnvil::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(BroadswordBlade);
	InteractTimer = 0;
}

void AForgeAnvil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HammeringMinigame(DeltaTime);
	FRotator rotate(0, 1,0);
	Rotator->AddWorldRotation(rotate);
}

// TEMP
#include "Engine.h"

void AForgeAnvil::ProcessPartItem(AForgePart * Part)
{
	if (Part->SwordPart != ESwordPart::PT_NONE || (Part->ResourceType != EResource::R_IRONINGOT && Part->ResourceType != EResource::R_STEELINGOT) || bHammerMinigamePlaying)
	{
		ThrowAway(Part);
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Part: " + Part->GetName()));
	switch (Part->ResourceType)
	{
	case(EResource::R_STEELINGOT):
		{
			CurrentResource = EBladeMat::BM_STEEL;
			RandomiseRange();
			bHammerMinigamePlaying = true;
			break;
		}
	case(EResource::R_IRONINGOT):
		{
			CurrentResource = EBladeMat::BM_IRON;
			RandomiseRange();
			bHammerMinigamePlaying = true;
			break;
		}
		default:
		{
			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResource"), true);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("No valid Resource type, instead is " + EnumPtr->GetNameByValue((int64)Part->ResourceType).ToString()));
			break;
		}
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	Part->Destroy();
}

void AForgeAnvil::HammeringMinigame(float Deltatime)
{
	if (SuccessHit || !bHammerMinigamePlaying) return;
	if (CurrentMarkerPos >= 1.0f) CurrentMarkerMoveSpeed = -MarkerMoveSpeed;
	else if (CurrentMarkerPos <= 0.0f) CurrentMarkerMoveSpeed = MarkerMoveSpeed;

	CurrentMarkerPos += CurrentMarkerMoveSpeed * Deltatime;
}

void AForgeAnvil::HammeringCycle(class AForgePlayer* Player)
{
	if (!Player->IsHoldingHammer) return;
	if (CurrentMarkerPos >= CurrentMinRange && CurrentMarkerPos <= CurrentMaxRange) // In range
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Success);
		SetOwner(Player);

		HammingCycles++;
		SuccessHit = true;
		GetWorldTimerManager().SetTimer(SuccessHitTimerHandle, this, &AForgeAnvil::SuccessTimeComplete, PauseTimeOnSuccess, false);
	}
	else // Missed, fail
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Failure, 1.0f, 1.0f, 0.0f);

		bHammerMinigamePlaying = false;
		HammingCycles = 0;
		CurrentResource = EBladeMat::BM_NONE;
		CurrentlyProcessing = EResource::R_NONE;
	}
}


void AForgeAnvil::SuccessTimeComplete()
{
	SuccessHit = false;
	if (HammingCycles >= MaxCycles)
	{
		HammingCycles = 0;
		MakeResource(CurrentResource);
		bHammerMinigamePlaying = false;
		CurrentResource = EBladeMat::BM_NONE;
		CurrentlyProcessing = EResource::R_NONE;
	}
	else
		RandomiseRange();

	GetWorldTimerManager().ClearTimer(SuccessHitTimerHandle);
}

void AForgeAnvil::RandomiseRange()
{
	float RangeSize = FMath::RandRange(RangeMin, RangeMax);
	CurrentMinRange = FMath::RandRange(0.0f, 1.0f - RangeSize);
	CurrentMaxRange = CurrentMinRange + RangeSize;
}

AForgePart * AForgeAnvil::MakeResource(EBladeMat type)
{
	if (!HasAuthority())
	{
		SERVER_MakeResource(type);
		return nullptr;
	}
	switch (type)
	{
		case(EBladeMat::BM_IRON):
		{
			if (CurrentState == EBladeType::BT_BROADSWORD)
			{
				AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronBroadBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
				return ResourceRef;
			}
			else
			{
				AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronKrisBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
				return ResourceRef;
			}
		}
		case(EBladeMat::BM_STEEL):
		{
			if (CurrentState == EBladeType::BT_BROADSWORD)
			{
				AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(SteelBroadBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
				return ResourceRef;
			}
			else
			{
				AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(SteelKrisBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
				return ResourceRef;
			}
		}
	}
	return nullptr;
}

void AForgeAnvil::SERVER_MakeResource_Implementation(EBladeMat type)
{
	MakeResource(type);
}
bool AForgeAnvil::SERVER_MakeResource_Validate(EBladeMat type)
{
	return true;
}

void AForgeAnvil::MorphStates(bool Next)
{
	if (bHammerMinigamePlaying) return;

	switch (CurrentState)
	{
	case EBladeType::BT_BROADSWORD:
	{
		if (Next)
			CurrentState = EBladeType::BT_KRIS;
		else
			CurrentState = EBladeType::BT_KRIS;
		break;
	}
	case EBladeType::BT_KRIS:
	{
		if (Next)
			CurrentState = EBladeType::BT_BROADSWORD;
		else
			CurrentState = EBladeType::BT_BROADSWORD;
		break;
	}
	}

	switch (CurrentState)
	{
	case EBladeType::BT_BROADSWORD:
		OutputName = FString("Broad Blade");
		CurrentProducingItem->SetStaticMesh(BroadswordBlade);
		break;
	case EBladeType::BT_KRIS:
		OutputName = FString("Kris Blade");
		CurrentProducingItem->SetStaticMesh(KrisSwordBlade);
		break;
	default:
		break;
	}
}

//void AForgeAnvil::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	//DOREPLIFETIME(AForgeAnvil, CurrentMinRange);
//	//DOREPLIFETIME(AForgeAnvil, CurrentMaxRange);
//	//DOREPLIFETIME(AForgeAnvil, CurrentMarkerPos);
//}