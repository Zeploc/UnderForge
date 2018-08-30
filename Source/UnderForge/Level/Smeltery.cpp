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


void ASmeltery::SmeltingMinigame(float DeltaTime)
{
	if (bSmeltingMinigamePlaying)
	{
		SmeltingTimePassed += DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, "Delta" + FString::SanitizeFloat(SmeltingTimePassed));
		if (SmeltingTimePassed > 10.0f)
		{
			bSmeltingMinigamePlaying = false;
			SmeltingTimePassed = 0.0f;
			CurrentlyProcessing = EResource::R_NONE;
			UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
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
			MakeMat(EResource::R_IRON);
			break;
		case(EResource::R_BRONZE):
			MakeMat(EResource::R_BRONZE);
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
		UE_LOG(LogTemp, Warning, TEXT("KABOOM"));
		//KABOOM
	}
	SmeltingTimePassed = 0.0f;
}

AForgePart * ASmeltery::MakeResource(EResource type)
{

	switch (type)
	{
		case(EResource::R_STEEL):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronForgedPart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}

		case(EResource::R_REALBRONZE):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(BronzeForgedPart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	return nullptr;
}

AForgeMat * ASmeltery::MakeMat(EResource type)
{

	switch (type)
	{
	case(EResource::R_IRON):
	{
		AForgeMat * ResourceRef = GetWorld()->SpawnActor<AForgeMat>(IronMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}

	case(EResource::R_BRONZE):
	{
		AForgeMat * ResourceRef = GetWorld()->SpawnActor<AForgeMat>(BronzeMat, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
		return ResourceRef;
	}
	}
	return nullptr;
}
