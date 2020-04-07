// Fill out your copyright notice in the Description page of Project Settings.

#include "CarpentaryStation.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
//#include "Items/Sword/HandlePart.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Utlities.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Player/ForgePlayer.h"
#include <string>
#include <cmath>

#include "ForgeAnvil.h"
#include "UnderForgeSingleton.h"



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
	CurrentResource = EResource::R_NONE;
	OutputName = FString("Rough Hilt");
	PotentiallyInteracting = false;
	SpinningRequired = 25.0f;
}

// Called when the game starts or when spawned
void ACarpentaryStation::BeginPlay()
{
	Super::BeginPlay();
	//CurrentProducingItem->SetStaticMesh(BroadSwordHandle);
}

// Called every frame
void ACarpentaryStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rotate(0,1,0);
	Rotator->AddWorldRotation(rotate);
	SpinningMinigame();
}

bool ACarpentaryStation::CanTakeMatItem(AForgeMat * material)
{
	return material->ResourceType == EResource::R_WOOD && CurrentResource == EResource::R_NONE;
}

bool ACarpentaryStation::ProcessMatItem(AForgeMat* material)
{
	CurrentPlayer = material->HeldPlayer;
	SetOwner(CurrentPlayer);
	CurrentResource = material->ResourceType;
	BeginMinigame();
	material->Destroy();
		
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	return true;
}

void ACarpentaryStation::Interacted(AForgePlayer * _Player)
{
	SetOwner(_Player);
}

void ACarpentaryStation::SERVER_SetCurrent_Implementation(float _x, float _y)
{
	MULTI_SetCurrent(_x, _y);
}
bool ACarpentaryStation::SERVER_SetCurrent_Validate(float _x, float _y)
{
	return true;
}

void ACarpentaryStation::MULTI_SetCurrent_Implementation(float _x, float _y)
{
	CurrentX = _x;
	CurrentY = _y;
}

AForgePart * ACarpentaryStation::MakeResource(EResource type)
{
	if (!HasAuthority())
	{
		SERVER_MakeResource(type);
		return nullptr;
	}
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = CurrentPlayer;
	CurrentPlayer = nullptr;*/
	EWeaponPart CreatedPart = AForgeAnvil::FindPartFromResource(type);

	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return nullptr;

	FWeaponPart* FoundPart = GameSingleton->Parts.Find(CreatedPart);
	if (FoundPart)
	{
		AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(FoundPart->PartClass, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());// , SpawnParams);
		return ResourceRef;
	}

	//switch (type)
	//{
	//	case EHandleType::HT_BROADSWORD:
	//	{
	//		AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(BroadswordHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());// , SpawnParams);
	//		return ResourceRef;
	//	}
	//	case EHandleType::HT_KRIS:
	//	{
	//		AForgePart* ResourceRef = GetWorld()->SpawnActor<AForgePart>(KrisHandlePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());//, SpawnParams);
	//		return ResourceRef;
	//	}
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "DIDN'T MAKE A DAMN PART");
	return nullptr;
}

void ACarpentaryStation::SERVER_MakeResource_Implementation(EResource type)
{
	MakeResource(type);
}
bool ACarpentaryStation::SERVER_MakeResource_Validate(EResource type)
{
	return true;
}

//void ACarpentaryStation::SetXValue(float x)
//{
//	CurrentX = x;
//	return;
//}
//
//void ACarpentaryStation::SetYValue(float y)
//{
//	CurrentY = y;
//	return;
//}


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
		if (CurrentPlayer && CurrentPlayer->IsLocallyControlled())
		{
			CurrentX = CurrentPlayer->CurrentX;// GetInputAxisKeyValue("SpinningXAxis");
			CurrentY = CurrentPlayer->CurrentY;// GetInputAxisKeyValue("SpinningYAxis");
			SERVER_SetCurrent(CurrentX, CurrentY);
		}

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
			if (CurrentPlayer && CurrentPlayer->IsLocallyControlled())
			{
				MakeResource(CurrentResource);
			}
			CurrentResource = EResource::R_NONE;
		}
	}
	return;
}