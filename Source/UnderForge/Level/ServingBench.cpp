 // Fill out your copyright notice in the Description page of Project Settings.

#include "ServingBench.h"

#include "Items/ForgeItem.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/World.h"

void AServingBench::ItemDectection(AActor* Item, bool entering)
{
	if (entering)
	{
		if (AForgeItem* ForgeItem = Cast<AForgeItem>(Item))
		{
			ForgeItems.Add(ForgeItem);
			ForgeItem->ItemMesh->SetSimulatePhysics(false);
			NewItem(ForgeItem);
		}
	}
}
