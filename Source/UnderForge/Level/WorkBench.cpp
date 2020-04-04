// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkBench.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"

AWorkBench::AWorkBench()
{

}

void AWorkBench::ProcessItem(class APickUpItem* Item)
{
	if (!CurrentItem)
	{
		if (HasAuthority())
		{
			MULTI_NewItem(Item);
		}
		else
		{
			CurrentItem = Item;
			CurrentItem->SetActorLocation(ObjectPosition->GetComponentLocation());
			CurrentItem->SetActorRotation(ObjectPosition->GetComponentRotation());
			CurrentItem->ItemMesh->SetSimulatePhysics(false);
		}
	}
	else
		ThrowAway(Item);
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
	CurrentItem = Item;
	CurrentItem->SetActorLocation(ObjectPosition->GetComponentLocation());
	CurrentItem->SetActorRotation(ObjectPosition->GetComponentRotation());
	CurrentItem->ItemMesh->SetSimulatePhysics(false);
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessInteractSound);
}
