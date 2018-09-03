// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"

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

	CurrentResource = EBladeMat::BM_NONE;
}
void AForgeAnvil::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(BroadswordBlade);
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
	Part->Destroy();
}

void AForgeAnvil::HammeringMinigame(float Deltatime)
{
	if (SuccessHit || !bHammerMinigamePlaying) return;
	if (CurrentMarkerPos >= 1.0f) CurrentMarkerMoveSpeed = -MarkerMoveSpeed;
	else if (CurrentMarkerPos <= 0.0f) CurrentMarkerMoveSpeed = MarkerMoveSpeed;

	CurrentMarkerPos += CurrentMarkerMoveSpeed * Deltatime;
}

void AForgeAnvil::HammeringCycle()
{
	if (CurrentMarkerPos >= CurrentMinRange && CurrentMarkerPos <= CurrentMaxRange) // In range
	{
		// Play success hit sound (or in animation)
		HammingCycles++;
		SuccessHit = true;
		GetWorldTimerManager().SetTimer(SuccessHitTimerHandle, this, &AForgeAnvil::SuccessTimeComplete, PauseTimeOnSuccess, false);
	}
	else // Missed, fail
	{
		// Play fail sound
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
		CurrentProducingItem->SetStaticMesh(BroadswordBlade);
		break;
	case EBladeType::BT_KRIS:
		CurrentProducingItem->SetStaticMesh(KrisSwordBlade);
		break;
	default:
		break;
	}
}
