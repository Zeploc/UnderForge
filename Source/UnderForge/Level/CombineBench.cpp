// Fill out your copyright notice in the Description page of Project Settings.

#include "CombineBench.h"

#include "Items/ForgePart.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ACombineBench::ACombineBench()
{
	CurrentWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	ObjectPosition->SetupAttachment(CurrentWeaponMesh);
}

void ACombineBench::ItemDectection(class AActor* OverlappActor, bool entering)
{
	if (AForgePart* Part = Cast<AForgePart>(OverlappActor))
	{
		if (!entering)
		{
			Part->CurrentTouchingStation = nullptr;
			return;
		}

		for (int i = 0; i < CurrentParts.Num(); i++)
		{
			if (CurrentParts[i]->PartType == Part->PartType) // Check if it already has that part type
			{
				ThrowAway(OverlappActor); // if it does throw it away
				return; // Stop checking
			}
		}
		Part->CurrentTouchingStation = this;
	}
	else
	{
		ThrowAway(OverlappActor);
		// Throw Array
	}
	
}

void ACombineBench::ChangeMesh()
{

}

void ACombineBench::ThrowAway(AActor * Actor)
{
	// YEET THAT BOI
}

void ACombineBench::ProcessPartItem(AForgePart * Part)
{
	CurrentParts.Add(Part);
	Part->DisableComponentsSimulatePhysics();
	Part->PartMesh->SetVisibility(false);
	Part->PartMesh->SetCollisionProfileName("OverlapAll");
	ChangeMesh();
}
