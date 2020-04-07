// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBench.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Player/ForgePlayer.h"

#include "Engine.h"

AWorkBench::AWorkBench()
{
	ValidTypes = { EPickUpType::PT_MAT, EPickUpType::PT_PART, EPickUpType::PT_WEAPON, EPickUpType::PT_TOOL };
}

bool AWorkBench::CanTakeItem(APickUpItem * Item)
{
	if (!CurrentItem && Item)
	{
		if (ValidTypes.Contains(Item->PickUpType))
			return true;
	}
	return false;
}

void AWorkBench::ProcessItem(class APickUpItem* Item)
{
	if (!CurrentItem && Item)
	{
		SetCurrentItem(Item);
		//if (HasAuthority())
		//{
		//	MULTI_NewItem(Item);
		//	/*if (CurrentItem->HeldPlayer)
		//	{
		//		CurrentItem->HeldPlayer->ClearHoldItem();
		//	}*/
		//}
		//else
		//{
		//	SetCurrentItem(Item);
		//	/*if (CurrentItem->HeldPlayer)
		//	{
		//		CurrentItem->HeldPlayer->ClearHoldItem();
		//	}*/
		//}
	}
	else
	{
		ThrowAway(Item);
		return;
	}
}

void AWorkBench::ItemPickedUp(APickUpItem * Item)
{
	Super::ItemPickedUp(Item);
	if (Item == CurrentItem)
	{
		CurrentItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CurrentItem = nullptr;
		UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
	}
}

void AWorkBench::MULTI_NewItem_Implementation(APickUpItem * Item)
{
	if (!Item)
		return;

	SetCurrentItem(Item);
}
void AWorkBench::SetCurrentItem(APickUpItem * Item)
{
	if (!Item)
		return;
	CurrentItem = Item;
	if (CurrentItem->HeldPlayer)
	{
		CurrentItem->HeldPlayer->LocalClearHoldItem();
	}
	CurrentItem->HeldPlayer = nullptr;
	CurrentItem->CurrentStation = this;
	CurrentItem->ItemMesh->SetSimulatePhysics(false);
	CurrentItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CurrentItem->ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CurrentItem->ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CurrentItem->SetActorLocation(ObjectPosition->GetComponentLocation());
	CurrentItem->SetActorRotation(ObjectPosition->GetComponentRotation());
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
}