// Fill out your copyright notice in the Description page of Project Settings.

#include "CombineBench.h"

#include "Items/ForgePart.h"
#include "Items/ForgeItem.h"
#include "Items/Sword/SwordItem.h"
#include "Utlities.h"
#include "Engine/World.h"

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
		Part->CurrentTouchingStation = this;
	}
	
}

void ACombineBench::ChangeMesh()
{

}

void ACombineBench::ProcessPartItem(AForgePart * Part)
{
	if (Part->PartType != EPartType::PT_BLADE && Part->PartType != EPartType::PT_HANDLE) // Check if its not a valid part
	{
		ThrowAway(Part); // if it does throw it away
		return; // Stop checking
	}

	if (CurrentItem == nullptr) // No current item/parts
	{
		if (Part->PartType == EPartType::PT_BLADE || Part->PartType == EPartType::PT_HANDLE) // is sword part
		{
			ESwordPart SwordPart = UUtilities::GetSwordPartEnum(Part);
			ASwordItem* NewSwordItem = GetWorld()->SpawnActor<ASwordItem>(ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			NewSwordItem->ForgeParts.Add(SwordPart);
			CurrentItem = NewSwordItem;
		}
		else // Is not valid part
		{
			ThrowAway(Part); // throw it away
		}
	}
	else // item exists, check valid part
	{
		if (ASwordItem* CurrentSwordItem = Cast<ASwordItem>(CurrentItem)) // Item is a sword
		{
			ESwordPart SwordPart = UUtilities::GetSwordPartEnum(Part);
			if (CurrentSwordItem->CanHavePart(SwordPart)) // If the sword needs that type of part
			{
				CurrentSwordItem->ForgeParts.Add(SwordPart); // Add the part
				Part->Destroy(); // Destroy part actor
				ChangeMesh(); // Update mesh
			}
			else
			{
				ThrowAway(Part); // if it does throw it away
				return; // Stop checking
			}
		}
		else // Is not valid part
		{
			ThrowAway(Part); // throw it away
		}
	}	
}
