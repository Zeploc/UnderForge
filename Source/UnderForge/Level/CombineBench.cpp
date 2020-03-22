// Fill out your copyright notice in the Description page of Project Settings.

#include "CombineBench.h"

#include "Items/ForgePart.h"
#include "Items/ForgeItem.h"
#include "Items/Sword/SwordItem.h"
#include "Utlities.h"
#include "Engine/World.h"

#include "UnrealNetwork.h"

#include "Components/StaticMeshComponent.h"

// TEMP
#include "Engine.h"

// Sets default values
ACombineBench::ACombineBench()
{
	ObjectPosition->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
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

void ACombineBench::ProcessPartItem(AForgePart * Part)
{
	if (Part->SwordPart == ESwordPart::PT_NONE) // Check if its not a valid part
	{
		ThrowAway(Part); // if it does throw it away
		return; // Stop checking
	}
	if (CurrentItem == nullptr) // No current item/parts
	{
		if (HasAuthority())
		{
			ASwordItem* NewSwordItem = GetWorld()->SpawnActor<ASwordItem>(SwordItem, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			NewSwordItem->ItemMesh->SetSimulatePhysics(false);
			NewSwordItem->AddPart(Part->SwordPart);
			CurrentParts.Add(Part->SwordPart);
			CurrentItem = NewSwordItem;
		}		
		Part->Destroy();
		UGameplayStatics::PlaySound2D(GetWorld(), NewItemAddedSound);
	}
	else // item exists, check valid part
	{
		if (ASwordItem* CurrentSwordItem = Cast<ASwordItem>(CurrentItem)) // Item is a sword
		{
			if (CurrentSwordItem->CanHavePart(Part->SwordPart)) // If the sword needs that type of part
			{
				if (CurrentSwordItem->AddPart(Part->SwordPart)) // Add the part
				{
					CurrentParts.Empty();
					CurrentItem->CanBePickedUp = true;
					// Is complete
					CurrentItem = nullptr;
				}
				Part->Destroy(); // Destroy part actor
				UGameplayStatics::PlaySound2D(GetWorld(), NewItemAddedSound);
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

void ACombineBench::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACombineBench, CurrentItem);
	DOREPLIFETIME(ACombineBench, CurrentParts);
}