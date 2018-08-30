// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"
#include "Engine.h"


AForgeAnvil::AForgeAnvil()
{
	HammerTimeMax = 2.0f;
	HammerTimeNeeded = 1.0f;
	HammerTimeKABOOM = 3.0f;
	HammingCycles = 0;
	MaxCycles = 5;

	bHammerMinigamePlaying = false;
	HammerTimePassed = 0.0f;

	StationMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	StationMesh2->SetupAttachment(StationMesh);
	StationMesh2->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StationMesh2->SetVisibility(false, false);

	CurrentResource = EBladeMat::BM_NONE;
}
void AForgeAnvil::BeginPlay()
{
	Super::BeginPlay();
}

void AForgeAnvil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HammeringMinigame(DeltaTime);
}

// TEMP
#include "Engine.h"

void AForgeAnvil::ItemDectection(AActor * OverlappActor, bool entering)
{
	if (AForgePart* Part = Cast<AForgePart>(OverlappActor))
	{
		if (Part->PartType == EPartType::PT_UNFORMEDBLADE)
		{
			if (!entering)
			{
				Part->CurrentTouchingStation = nullptr;
				return;
			}
			Part->CurrentTouchingStation = this;
		}
		else
			ThrowAway(OverlappActor);
	}
	else
		ThrowAway(OverlappActor);
}

void AForgeAnvil::ProcessPartItem(AForgePart * Part)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Part: " + Part->GetName()));
	switch (Part->ResourceType)
	{
		case(EResource::R_REALBRONZE):
		{
			CurrentResource = EBladeMat::BM_BRONZE;
			bHammerMinigamePlaying = true;
			break;
		}
		case(EResource::R_STEEL):
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
	HammerTimePassed = 0.0f;
	HammingCycles++;
	if (HammingCycles >= MaxCycles)
	{
		MakeResource(CurrentResource);
		CurrentResource = EBladeMat::BM_NONE;
		bHammerMinigamePlaying = false;
		HammerTimePassed = 0.0f;
		StationMesh2->SetVisibility(false, false);
	}
}

AForgePart * AForgeAnvil::MakeResource(EBladeMat type)
{
	switch (type)
	{
		case(EBladeMat::BM_IRON):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
		case(EBladeMat::BM_BRONZE):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(BronzeBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	return nullptr;
}
