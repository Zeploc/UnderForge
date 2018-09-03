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

// TEMP
#include "Engine.h"

ACarpentaryStation::ACarpentaryStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating"));
	Rotator->SetupAttachment(StationMesh);
	Rotator->SetRelativeLocation(StationMesh->RelativeLocation);
	Rotator->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CurrentProducingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	CurrentProducingItem->SetupAttachment(Rotator);
	CurrentProducingItem->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	CurrentProducingItem->SetVisibility(true, false);
	CurrentState = EHandleType::HT_BROADSWORD;

	PotentiallyInteracting = false;
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
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
	if (material->ResourceType == EResource::R_WOOD)
	{
		MakeResource(CurrentState);
		material->Destroy();
	}
	else
		ThrowAway(material);	
}

void ACarpentaryStation::MorphStates()
{
	if (CurrentState == EHandleType::HT_BROADSWORD)
	{
		CurrentState = EHandleType::HT_KRIS;
		CurrentProducingItem->SetStaticMesh(KrisSwordHandle);
	}
	else if(CurrentState == EHandleType::HT_KRIS)
	{
		CurrentState = EHandleType::HT_BROADSWORD;
		CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
	}
}

AForgePart * ACarpentaryStation::MakeResource(EHandleType type)
{
	switch (type)
	{
		case EHandleType::HT_BROADSWORD:
		{
			AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(BroadswordHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
		case EHandleType::HT_KRIS:
		{
			AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(KrisHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "DIDN'T MAKE A DAMN PART");
	return nullptr;
}

void ACarpentaryStation::BeginMinigame(float x, float y)
{

}

void ACarpentaryStation::SpinningMinigame()
{

}
