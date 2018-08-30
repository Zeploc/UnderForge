// Fill out your copyright notice in the Description page of Project Settings.

#include "CarpentaryStation.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Items/Sword/HandlePart.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Utlities.h"

ACarpentaryStation::ACarpentaryStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StationMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	StationMesh2->SetupAttachment(StationMesh);
	StationMesh2->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	CurrentState = 1;
	PotentiallyInteracting = false;
	StationMesh2->SetVisibility(false, false);
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACarpentaryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarpentaryStation::ProcessMatItem(AForgeMat* material)
{
	material->Destroy();
	if (CurrentState == 1)
	{
		MakeResource(EHandleType::HT_CURVED);
	}
	else if (CurrentState == 2)
	{
		MakeResource(EHandleType::HT_STRAIGHT);
	}
}

void ACarpentaryStation::ItemDectection(AActor* actor, bool entering)
{
	if (AForgeMat* mat = Cast<AForgeMat>(actor))
	{
		if (entering)
		{
			if (mat->ResourceType == EResource::R_WOOD )
			{
				mat->CurrentTouchingStation = this;
				PotentiallyInteracting = true;
			}
		}
		else
		{
			if (mat->ResourceType == EResource::R_WOOD)
			{
				mat->CurrentTouchingStation = nullptr;
				PotentiallyInteracting = false;
			}
		}
	}
}

void ACarpentaryStation::MorphStates()
{
	if (CurrentState == 1)
	{
		CurrentState = 2;
		StationMesh2->SetVisibility(true, false);
		StationMesh->SetVisibility(false, false);
	}
	else if(CurrentState == 2)
	{
		CurrentState = 1;
		StationMesh2->SetVisibility(false, false);
		StationMesh->SetVisibility(true, false);
	}
}

AForgePart * ACarpentaryStation::MakeResource(EHandleType type)
{
	switch (type)
	{
	case EHandleType::HT_STRAIGHT:
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AHandlePart>(StraightHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	case EHandleType::HT_CURVED:
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AHandlePart>(CurvedHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	}
	return nullptr;
}

