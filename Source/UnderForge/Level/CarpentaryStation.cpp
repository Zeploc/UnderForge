// Fill out your copyright notice in the Description page of Project Settings.

#include "CarpentaryStation.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Items/Sword/HandlePart.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Utlities.h"

ACarpentaryStation::ACarpentaryStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating"));
	Rotator->SetupAttachment(StationMesh);
	Rotator->SetRelativeLocation(StationMesh->RelativeLocation);

	CurrentProducingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	CurrentProducingItem->SetupAttachment(Rotator);
	CurrentProducingItem->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	CurrentProducingItem->SetVisibility(true, false);
	CurrentState = 1;

	PotentiallyInteracting = false;
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
	Rotator->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
}

// Called every frame
void ACarpentaryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rotate(0,1,0);
	Rotator->AddWorldRotation(rotate);
}

void ACarpentaryStation::ProcessMatItem(AForgeMat* material)
{
	material->Destroy();
	if (CurrentState == 1)
	{
		MakeResource(EHandleType::HT_BROADSWORD);
	}
	else if (CurrentState == 2)
	{
		MakeResource(EHandleType::HT_KRIS);
	}
}

void ACarpentaryStation::ItemDectection(AActor* actor, bool entering)
{
	if (AForgeMat* mat = Cast<AForgeMat>(actor))
	{

		if (mat->ResourceType == EResource::R_WOOD)
		{
			if (entering)
			{
				mat->CurrentTouchingStation = this;
				PotentiallyInteracting = true;
			}
			else
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
		CurrentProducingItem->SetStaticMesh(KrisSwordHandle);
	}
	else if(CurrentState == 2)
	{
		CurrentState = 1;
		CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
	}
}

AForgePart * ACarpentaryStation::MakeResource(EHandleType type)
{
	switch (type)
	{
	case EHandleType::HT_BROADSWORD:
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AHandlePart>(BroadswordHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	case EHandleType::HT_KRIS:
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AHandlePart>(KrisHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	}
	return nullptr;
}

