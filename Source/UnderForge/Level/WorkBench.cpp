// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBench.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Player/ForgePlayer.h"

AWorkBench::AWorkBench()
{

}

bool AWorkBench::ProcessItem(class APickUpItem* Item)
{
	if (!CurrentItem)
	{
		if (HasAuthority())
		{
			MULTI_NewItem(Item);
			if (CurrentItem->HeldPlayer)
			{
				CurrentItem->HeldPlayer->ClearHoldItem();
			}
		}
		else
		{
			SetCurrentItem(Item);
			if (CurrentItem->HeldPlayer)
			{
				CurrentItem->HeldPlayer->ClearHoldItem();
			}
		}
	}
	else
	{
		ThrowAway(Item);
		return false;
	}
	return true;
}

void AWorkBench::ItemPickedUp(APickUpItem * Item)
{
	Super::ItemPickedUp(Item);
	if (Item == CurrentItem)
	{
		CurrentItem = nullptr;
		UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	}
}

void AWorkBench::MULTI_NewItem_Implementation(APickUpItem * Item)
{
	if (!Item)
		return;

	SetCurrentItem(Item);
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
}
void AWorkBench::SetCurrentItem(APickUpItem * Item)
{
	CurrentItem = Item;
	CurrentItem->SetActorLocation(ObjectPosition->GetComponentLocation());
	CurrentItem->SetActorRotation(ObjectPosition->GetComponentRotation());
	CurrentItem->HeldPlayer = nullptr;
	CurrentItem->ItemMesh->SetSimulatePhysics(false);
	CurrentItem->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CurrentItem->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CurrentItem->CurrentStation = this;
}