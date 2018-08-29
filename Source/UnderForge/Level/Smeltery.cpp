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
}

void ASmeltery::ProcessMatItem(AForgeMat* material)
{
	switch (material->ResourceType)
	{
	case(EResource::R_IRON):
		material->Destroy();
		MakeResource(EResource::R_STEEL);
		break;

	case(EResource::R_BRONZE):
		material->Destroy();
		MakeResource(EResource::R_REALBRONZE);
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