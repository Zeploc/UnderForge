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
			}
		}
		else
		{
			if (mat->ResourceType == EResource::R_WOOD)
			{
				mat->CurrentTouchingStation = nullptr;
			}
		}
	}
}

AForgePart * ACarpentaryStation::MakeResource(EResource type)
{
	return nullptr;
}

