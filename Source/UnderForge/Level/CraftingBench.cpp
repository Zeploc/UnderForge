// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingBench.h"

#include "Items/ForgePart.h"
#include "Items/ForgeItem.h"
#include "Items/Sword/SwordItem.h"
#include "Utlities.h"
#include "UnderForgeSingleton.h"
#include "Engine/World.h"

#include "UnrealNetwork.h"

#include "Components/StaticMeshComponent.h"

// TEMP
#include "Engine.h"

// Sets default values
ACraftingBench::ACraftingBench()
{
	ObjectPosition->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
}


bool ACraftingBench::CanTakePartItem(AForgePart * Part)
{
	if (Part->SwordPart == EWeaponPart::WP_NONE) // Check if its not a valid part
		return false;
	if (CurrentItem == nullptr) // No current item/parts
		return true;
	if (CurrentItem->CanHavePart(Part->SwordPart)) // If the sword needs that type of part
		return true;
	return false;
}

bool ACraftingBench::ProcessPartItem(AForgePart * Part)
{
	if (Part->SwordPart == EWeaponPart::WP_NONE) // Check if its not a valid part
	{
		ThrowAway(Part); // if it does throw it away
		return false; // Stop checking
	}
	if (CurrentItem == nullptr) // No current item/parts
	{
		if (HasAuthority())
		{
			ASwordItem* NewSwordItem = GetWorld()->SpawnActor<ASwordItem>(SwordItem, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			NewSwordItem->ItemMesh->SetSimulatePhysics(false);
			NewSwordItem->AddPart(Part->SwordPart);
			CurrentItem = NewSwordItem;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Server: New Item");
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "All: New Item");
		Part->Destroy();
		UGameplayStatics::PlaySound2D(GetWorld(), NewItemAddedSound);
	}
	else // item exists, check valid part
	{
		if (CurrentItem->CanHavePart(Part->SwordPart)) // If the sword needs that type of part
		{
			if (HasAuthority())
			{
				if (CurrentItem->AddPart(Part->SwordPart)) // Add the part
				{
					CurrentItem->CanBePickedUp = true;
					// Is complete
					CurrentItem = nullptr;
				}
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Server: Add Part");
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "All: Add Part");
			Part->Destroy(); // Destroy part actor
			UGameplayStatics::PlaySound2D(GetWorld(), NewItemAddedSound);
		}
		else
		{
			ThrowAway(Part); // if it does throw it away
			return false; // Stop checking
		}
	}	

	return true;
}
void ACraftingBench::S_Disassemble()
{
	if (!HasAuthority())
		return;

	if (!CurrentItem)
		return;

	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (GameSingleton && GetWorld())
	{
		for (EWeaponPart WeaponPart : CurrentItem->ForgeParts)
		{
			FWeaponPart* FoundPart = GameSingleton->Parts.Find(WeaponPart);
			if (FoundPart)
			{
				if (!FoundPart->PartClass)
					continue;

				AForgePart* NewPart = GetWorld()->SpawnActor<AForgePart>(FoundPart->PartClass, RefuseThrowDirection->GetComponentLocation(), RefuseThrowDirection->GetComponentRotation());
				ThrowAway(NewPart); // Throw out
			}			
		}
	}


	CurrentItem->Disassemble();
	CurrentItem = nullptr;
}

void ACraftingBench::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingBench, CurrentItem);
}