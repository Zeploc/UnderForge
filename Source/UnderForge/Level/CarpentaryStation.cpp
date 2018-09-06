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
#include "Runtime/Core/Public/Math/Vector.h"
#include <string>
#include <cmath>



#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
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
	OutputName = FString("Rough Hilt");
	PotentiallyInteracting = false;
	SpinningRequired = 25.0f;
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
	CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
	InteractTimer = 0;
}

// Called every frame
void ACarpentaryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rotate(0,1,0);
	Rotator->AddWorldRotation(rotate);
	SpinningMinigame();
}

void ACarpentaryStation::ProcessMatItem(AForgeMat* material)
{
	if (material->ResourceType == EResource::R_WOOD)
	{
		BeginMinigame();
		material->Destroy();
	}
	else
		ThrowAway(material);	
}

void ACarpentaryStation::MorphStates(bool Next)
{
	if (!bSpinningGamePlaying)
	{
		switch (CurrentState)
		{
		case EHandleType::HT_BROADSWORD:
		{
			if (Next)
				CurrentState = EHandleType::HT_KRIS;
			else
				CurrentState = EHandleType::HT_KRIS;
			break;
		}
		case EHandleType::HT_KRIS:
		{
			if (Next)
				CurrentState = EHandleType::HT_BROADSWORD;
			else
				CurrentState = EHandleType::HT_BROADSWORD;
			break;
		}
		}

		switch (CurrentState)
		{
		case EHandleType::HT_BROADSWORD:
			OutputName = FString("Rough Hilt");
			CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
			break;
		case EHandleType::HT_KRIS:
			OutputName = FString("Smooth Hilt");
			CurrentProducingItem->SetStaticMesh(KrisSwordHandle);
			break;
		default:
			break;
		}
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

void ACarpentaryStation::SetXValue(float x)
{
	CurrentX = x;
	return;
}

void ACarpentaryStation::SetYValue(float y)
{
	CurrentY = y;
	return;
}


void ACarpentaryStation::BeginMinigame()
{
	bSpinningGamePlaying = true;
	PreviousX = CurrentX;
	PreviousY = CurrentY;
	SpinningTotal = 0.0f;
	return;
}

void ACarpentaryStation::SpinningMinigame()
{
	if (bSpinningGamePlaying)
	{
		FVector currentStickPos(CurrentX, CurrentY, 0.0f);
		currentStickPos.Normalize();
		FVector PreviousStickPos(PreviousX, PreviousY, 0.0f);
		PreviousStickPos.Normalize();
		float change = FVector::DotProduct(PreviousStickPos,currentStickPos);
		if (change > 0.0f)
		{
			change = abs(1.0f - change);
			SpinningTotal += change;
			//FString TheFloatStr = FString::SanitizeFloat(SpinningTotal);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Total: " + TheFloatStr));
		}
		PreviousX = CurrentX;
		PreviousY = CurrentY;
		if (SpinningTotal > SpinningRequired)
		{
			SpinningTotal = 0.0f;
			bSpinningGamePlaying = false;
			MakeResource(CurrentState);
		}
	}
	return;
}