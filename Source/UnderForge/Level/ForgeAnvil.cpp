// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/ForgePart.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Player/ForgePlayer.h"
#include "UnderForgeSingleton.h"

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
	//CurrentState =  EBladeType::BT_BROADSWORD;
	OutputName = FString("Broad Blade");
	CurrentResource = EResource::R_NONE;//EBladeMat::BM_NONE;

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundSuccess(TEXT("SoundWave'/Game/Sounds/SoundRourke/SmithingHit_Sound.SmithingHit_Sound'"));
	Success = SoundSuccess.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFail(TEXT("SoundWave'/Game/Sounds/SoundRourke/Error_or_Drop_Sound.Error_or_Drop_Sound'"));
	Failure = SoundFail.Object;

	PotentiallyInteracting = false;

	AvailableResourceTypes = { EResource::R_IRONINGOT, EResource::R_STEELINGOT, EResource::R_BRONZEINGOT, EResource::R_COPPERINGOT, EResource::R_GOLDINGOT, EResource::R_SILVERINGOT, EResource::R_TININGOT };
}
void AForgeAnvil::BeginPlay()
{
	Super::BeginPlay();
	//CurrentProducingItem->SetStaticMesh(BroadswordBlade);
	InteractTimer = 0;

	CurrentOrb = 0;// OrbCount / 2;
}
bool AForgeAnvil::TryInteract(AForgePlayer * _Player)
{
	SetOwner(_Player);
	//UE_LOG(LogTemp, Warning, TEXT("Minigame Start"));
	if (bHammerMinigamePlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Minigame Interact"));
		if (_Player->HoldingHammer() && _Player->IsLocallyControlled())
		{
			_Player->StartAttack();
		}
		//Interacted(_Player);
		return true;
	}
	else
	{
		return Super::TryInteract(_Player);
	}
	return false;
}


void AForgeAnvil::HitWithHammer(AForgePlayer * _Player)
{
	if (!_Player->IsLocallyControlled())
		return;
	if (!_Player->HoldingHammer() || SuccessHit) return;

	SERVER_InteractHit(true);
	//MULTI_InteractHit_Implementation(true);
	//GetWorldTimerManager().SetTimer(SuccessHitTimerHandle, this, &AForgeAnvil::SuccessTimeComplete, PauseTimeOnSuccess, false);
	//if (CurrentMarkerPos >= CurrentMinRange && CurrentMarkerPos <= CurrentMaxRange) // In range
	//{
	//	//SetOwner(_Player);
	//	SERVER_InteractHit(true);
	//	SuccessHit = true;

	//	GetWorldTimerManager().SetTimer(SuccessHitTimerHandle, this, &AForgeAnvil::SuccessTimeComplete, PauseTimeOnSuccess, false);
	//}
	//else // Missed, fail
	//{
	//	SERVER_InteractHit(false);

	//	if (CurrentOrb - 1 > 0)
	//	{
	//		RandomiseRange();
	//	}
	//}
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

bool AForgeAnvil::ProcessPartItem(AForgePart * Part)
{
	if (!Part)
		return false;
	if (Part->SwordPart != EWeaponPart::WP_NONE || !AvailableResourceTypes.Contains(Part->ResourceType) || bHammerMinigamePlaying)
	{
		ThrowAway(Part);
		return false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Part: " + Part->GetName()));
	CurrentResource = Part->ResourceType;
	RandomiseRange();
	bHammerMinigamePlaying = true;
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	Part->Destroy();
	return true;
}

void AForgeAnvil::SERVER_InteractHit_Implementation(bool _Success)
{
	MULTI_InteractHit(_Success);
}
bool AForgeAnvil::SERVER_InteractHit_Validate(bool _Success)
{
	return true;
}

void AForgeAnvil::MULTI_InteractHit_Implementation(bool _Success)
{
	UGameplayStatics::PlaySound2D(GetWorld(), _Success ? Success : Failure);
	if (_Success)
	{
		//SuccessHit = true;
		CurrentOrb++;
		//if (!SuccessHitTimerHandle.IsValid())
		//	GetWorldTimerManager().SetTimer(SuccessHitTimerHandle, this, &AForgeAnvil::SuccessTimeComplete, PauseTimeOnSuccess, false);
		if (CurrentOrb >= OrbCount)
		{
			if (HasAuthority())
				MakeResource(CurrentResource);

			CurrentOrb = 0;// OrbCount / 2;
			bHammerMinigamePlaying = false;
			CurrentResource = EResource::R_NONE;
			UGameplayStatics::PlaySound2D(GetWorld(), FullyCompletedCrafting);
		}
	}
	else
	{
		CurrentOrb--;
		if (CurrentOrb <= 0)
		{
			bHammerMinigamePlaying = false;
			CurrentOrb = 0;// OrbCount / 2;
			CurrentResource = EResource::R_NONE;
		}
		else
			RandomiseRange();
	}
}

void AForgeAnvil::HammeringMinigame(float Deltatime)
{
	if (SuccessHit || !bHammerMinigamePlaying) return;
	if (CurrentMarkerPos >= 1.0f) CurrentMarkerMoveSpeed = -MarkerMoveSpeed;
	else if (CurrentMarkerPos <= 0.0f) CurrentMarkerMoveSpeed = MarkerMoveSpeed;

	CurrentMarkerPos += CurrentMarkerMoveSpeed * Deltatime;
}

void AForgeAnvil::SuccessTimeComplete()
{
	SuccessHit = false;
	if (CurrentOrb >= OrbCount)
	{
		if (HasAuthority())
			MakeResource(CurrentResource);

		CurrentOrb = OrbCount / 2;
		bHammerMinigamePlaying = false;
		CurrentResource = EResource::R_NONE;
		UGameplayStatics::PlaySound2D(GetWorld(), FullyCompletedCrafting);
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

AForgePart * AForgeAnvil::MakeResource(EResource type)
{
	if (!HasAuthority())
	{
		SERVER_MakeResource(type);
		return nullptr;
	}

	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FWeaponPart* FoundWeaponPart = GameSingleton->Parts.Find(FindPartFromResource(type));
	if (!FoundWeaponPart)
		return nullptr;

	AForgePart * ForgePartRef = GetWorld()->SpawnActor<AForgePart>(FoundWeaponPart->PartClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
	return ForgePartRef;

}

EWeaponPart AForgeAnvil::FindPartFromResource(EResource _Resource)
{
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return EWeaponPart::WP_NONE;

	TArray<EWeaponPart> WeaponParts;
	GameSingleton->Parts.GenerateKeyArray(WeaponParts);
	for (EWeaponPart WeaponPart : WeaponParts)
	{
		FWeaponPart FoundWeaponpart = GameSingleton->Parts[WeaponPart];
		if (FoundWeaponpart.ResourcesRequired.Contains(_Resource))
		{
			return WeaponPart;
		}
	}
	return EWeaponPart::WP_NONE;
}

void AForgeAnvil::SERVER_MakeResource_Implementation(EResource type)
{
	MakeResource(type);
}
bool AForgeAnvil::SERVER_MakeResource_Validate(EResource type)
{
	return true;
}

void AForgeAnvil::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AForgeAnvil, CurrentMinRange);
	DOREPLIFETIME(AForgeAnvil, CurrentMaxRange);
	//DOREPLIFETIME(AForgeAnvil, CurrentMarkerPos);
}