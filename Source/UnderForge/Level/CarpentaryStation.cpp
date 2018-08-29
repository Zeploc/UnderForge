// Fill out your copyright notice in the Description page of Project Settings.

#include "CarpentaryStation.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Utlities.h"

ACarpentaryStation::ACarpentaryStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StationMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh2"));
	CurrentState = 1;
	PotentiallyInteracting = false;


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
	switch (material->ResourceType)
	{
	case(EResource::R_WOOD):
		material->Destroy();
		if (CurrentState == 1)
		{
			MakeResource(EResource::R_PROCESSEDWOOD);
		}
		else if (CurrentState == 2)
		{
			MakeResource(EResource::R_PROCESSEDWOOD2);
		}
		break;
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

AForgePart * ACarpentaryStation::MakeResource(EResource type)
{
	if (type == EResource::R_PROCESSEDWOOD)
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(ForgePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	else
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(ForgePart2, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
}

