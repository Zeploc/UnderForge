// Fill out your copyright notice in the Description page of Project Settings.

#include "Smeltery.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Utlities.h"
#include "Engine.h"

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
	if (!bSmeltingMinigamePlaying)
	{
		CurrentlyProcessing = material->ResourceType;
		material->Destroy();
		bSmeltingMinigamePlaying = true;
	}
}

void ASmeltery::ItemDectection(AActor* actor, bool entering)
{
	if (AForgeMat* mat = Cast<AForgeMat>(actor))
	{
		if (entering)
		{
			if (mat->ResourceType == EResource::R_IRONORE)
			{
				mat->CurrentTouchingStation = this;
			}
		}
		else
		{
			if (mat->ResourceType == EResource::R_IRONORE)
			{
				mat->CurrentTouchingStation = nullptr;
			}
		}
	}
}


void ASmeltery::SmeltingMinigame(float DeltaTime)
{
	if (bSmeltingMinigamePlaying)
	{
		SmeltingTimePassed += DeltaTime;
		CurrentRemainingTime = SmeltingTimeNeeded - SmeltingTimePassed;
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, "Delta" + FString::SanitizeFloat(SmeltingTimePassed));
		if (SmeltingTimePassed > SmeltingTimeKABOOM)
		{
			bSmeltingMinigamePlaying = false;
			SmeltingTimePassed = 0.0f;
			CurrentRemainingTime = 0.0f;
			CurrentlyProcessing = EResource::R_NONE;
			UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
			//KABOOM
		}
	}
}

void ASmeltery::MiniGameComplete()
{
	bSmeltingMinigamePlaying = false;
	if (SmeltingTimePassed < SmeltingTimeNeeded)
	{
		bSmeltingMinigamePlaying = false;
		MakeMat(CurrentlyProcessing);
		CurrentlyProcessing = EResource::R_NONE;
	}
	else if (SmeltingTimePassed < SmeltingTimeKABOOM)
	{
		bSmeltingMinigamePlaying = false;
		switch (CurrentlyProcessing)
		{
		case(EResource::R_IRONORE):
			MakeResource(EResource::R_IRONINGOT);
			break;
		}
		CurrentlyProcessing = EResource::R_NONE;
	}
	else
	{
		bSmeltingMinigamePlaying = false;
		CurrentlyProcessing = EResource::R_NONE;
		UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
		//KABOOM
	}
	SmeltingTimePassed = 0.0f;
}

AForgePart * ASmeltery::MakeResource(EResource type)
{

	switch (type)
	{
	case(EResource::R_IRONINGOT):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronIngotPart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}

	case(EResource::R_STEELINGOT):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(SteelForgedPart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	return nullptr;
}

AForgeMat * ASmeltery::MakeMat(EResource type)
{

	switch (type)
	{
		case(EResource::R_IRONORE):
		{
			AForgeMat * ResourceRef = GetWorld()->SpawnActor<AForgeMat>(IronMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	return nullptr;
}
