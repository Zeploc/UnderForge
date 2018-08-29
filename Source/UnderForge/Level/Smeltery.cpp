// Fill out your copyright notice in the Description page of Project Settings.

#include "Smeltery.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Utlities.h"

// Sets default values
ASmeltery::ASmeltery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SmeltingTimePassed = 0.0f;
	bSmeltingMinigamePlaying = false;
	SmeltingTimeKABOOM = 10.0f;
	SmeltingTimeNeeded = 5.0f;
	SmeltingTimeMax = 8.0f;
}

// Called when the game starts or when spawned
void ASmeltery::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASmeltery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SmeltingMinigame(DeltaTime);
}

void ASmeltery::ProcessMatItem(AForgeMat* material)
{
	switch (material->ResourceType)
	{
	case(EResource::R_IRON):
		CurrentlyProcessing = material->ResourceType;
		material->Destroy();
		bSmeltingMinigamePlaying = true;
		break;

	case(EResource::R_BRONZE):
		CurrentlyProcessing = material->ResourceType;
		material->Destroy();
		bSmeltingMinigamePlaying = true;
		break;
	}
}

void ASmeltery::ItemDectection(AActor* actor, bool entering)
{
	if (AForgeMat* mat = Cast<AForgeMat>(actor))
	{
		if (entering)
		{
			if (mat->ResourceType == EResource::R_IRON || mat->ResourceType == EResource::R_BRONZE)
			{
				mat->CurrentTouchingStation = this;
			}
		}
		else
		{
			if (mat->ResourceType == EResource::R_IRON || mat->ResourceType == EResource::R_BRONZE)
			{
				mat->CurrentTouchingStation = nullptr;
			}
		}
	}
}


AForgePart * ASmeltery::MakeResource(EResource type)
{
	AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(ForgedPart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
	return ResourceRef;
}

void ASmeltery::SmeltingMinigame(float DeltaTime)
{
	if (bSmeltingMinigamePlaying)
	{
		SmeltingTimePassed += DeltaTime;
		if (SmeltingTimePassed > 10.0f)
		{
			bSmeltingMinigamePlaying = false;
			CurrentlyProcessing = EResource::R_NONE;
			//KABOOM
		}
	}
}

void ASmeltery::MiniGameComplete()
{
	bSmeltingMinigamePlaying = false;
	if (SmeltingTimePassed < 5.0f)
	{
		bSmeltingMinigamePlaying = false;
		switch (CurrentlyProcessing)
		{
		case(EResource::R_IRON):
			MakeResource(EResource::R_IRON);
			break;
		case(EResource::R_BRONZE):
			MakeResource(EResource::R_BRONZE);
			break;
		}
		CurrentlyProcessing = EResource::R_NONE;
	}
	else if (SmeltingTimePassed < 8.0f)
	{
		bSmeltingMinigamePlaying = false;
		switch (CurrentlyProcessing)
		{
		case(EResource::R_IRON):
			MakeResource(EResource::R_STEEL);
			break;
		case(EResource::R_BRONZE):
			MakeResource(EResource::R_REALBRONZE);
			break;
		}
		CurrentlyProcessing = EResource::R_NONE;
	}
	else
	{
		bSmeltingMinigamePlaying = false;
		CurrentlyProcessing = EResource::R_NONE;
		//KABOOM
	}
}
