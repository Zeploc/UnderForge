// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SceneComponent.h"
#include <ctime>

AForgeAnvil::AForgeAnvil()
{
	srand(static_cast <unsigned> (time(0)));

	HammerTimeMax = 2.0f;
	HammerTimeNeeded = 1.0f;
	HammerTimeKABOOM = 3.0f;
	HammingCycles = 0;
	MaxCycles = 5;

	bHammerMinigamePlaying = false;
	HammerTimePassed = 0.0f;
	Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating"));
	Rotator->SetupAttachment(StationMesh);
	Rotator->SetRelativeLocation(StationMesh->RelativeLocation);

	StationMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	StationMesh2->SetupAttachment(StationMesh);
	StationMesh2->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StationMesh2->SetVisibility(false, false);

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
			bHammerMinigamePlaying = true;
			break;
		}
	case(EResource::R_IRONINGOT):
		{
			CurrentResource = EBladeMat::BM_IRON;
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
	if (bHammerMinigamePlaying)
	{
		HammerTimePassed += Deltatime;
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, "TimePassed: " + FString::SanitizeFloat(HammerTimePassed));
		if (HammerTimePassed > HammerTimeKABOOM)
		{
			bHammerMinigamePlaying = false;
			HammerTimePassed = 0.0f;
			HammerTimeMax = 2.0f;
			HammerTimeNeeded = 1.0f;
			CurrentResource = EBladeMat::BM_NONE;
			CurrentlyProcessing = EResource::R_NONE;
			UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
			//KABOOM
		}
		else if (HammerTimePassed > HammerTimeNeeded && HammerTimePassed < HammerTimeMax)
		{
			if (!StationMesh2->IsVisible())
			{
				StationMesh2->SetVisibility(true, false);
			}
		}
		else if (HammerTimePassed < HammerTimeNeeded || HammerTimePassed > HammerTimeMax)
		{
			if (StationMesh2->IsVisible())
			{
				StationMesh2->SetVisibility(false, false);
			}
		}
	}
}

void AForgeAnvil::HammeringCycle()
{
	if (HammerTimePassed > HammerTimeNeeded && HammerTimePassed < HammerTimeMax)
	{
		HammingCycles++;
		HammerTimePassed = 0.0f;
	}
	else
	{
		bHammerMinigamePlaying = false;
		CurrentResource = EBladeMat::BM_NONE;
		CurrentlyProcessing = EResource::R_NONE;
		HammerTimePassed = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("KABOOM"));


		HammingCycles = 0;
		//KABOOM
	}
	if (HammingCycles >= MaxCycles)
	{
		MakeResource(CurrentResource);
		bHammerMinigamePlaying = false;
		CurrentResource = EBladeMat::BM_NONE;
		CurrentlyProcessing = EResource::R_NONE;
		HammerTimePassed = 0.0f;
		HammerTimeMax = 2.0f;
		HammerTimeNeeded = 1.0f;
		StationMesh2->SetVisibility(false, false);
		HammingCycles = 0;
	}
	RandomizeHammeringWindow();
}

void AForgeAnvil::RandomizeHammeringWindow()
{
	float r = 0.5f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/ (2.6f - 0.5f));
	HammerTimeMax = r + 0.3f;
	HammerTimeNeeded = r - 0.3f;
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


void AForgeAnvil::MorphStates()
{
	if (bHammerMinigamePlaying) return;
	if (CurrentState == EBladeType::BT_BROADSWORD)
	{
		UE_LOG(LogTemp, Warning, TEXT("hi"));
		CurrentState = EBladeType::BT_KRIS;
		CurrentProducingItem->SetStaticMesh(KrisSwordBlade);
	}
	else if (CurrentState == EBladeType::BT_KRIS)
	{
		CurrentState = EBladeType::BT_BROADSWORD;
		CurrentProducingItem->SetStaticMesh(BroadswordBlade);
	}
}
