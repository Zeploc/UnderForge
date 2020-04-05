// Fill out your copyright notice in the Description page of Project Settings.


#include "Incinerator.h"

#include "Items/PickUpItem.h"
#include "Player/ForgePlayer.h"


bool AIncinerator::CanTakeItem(APickUpItem * Item)
{
	return Item->PickUpType == EPickUpType::PT_MAT || Item->PickUpType == EPickUpType::PT_PART;
}

void AIncinerator::ProcessItem(APickUpItem * Item)
{
	if (Item->HeldPlayer)
	{
		Item->HeldPlayer->LocalClearHoldItem();
	}
	Item->Destroy();
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
}
