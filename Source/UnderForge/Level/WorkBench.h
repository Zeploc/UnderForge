// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "WorkBench.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AWorkBench : public AForgeStation
{
	GENERATED_BODY()

public:
	AWorkBench();


public:
	virtual bool CanTakeItem(class APickUpItem* Item) override;
	virtual void ProcessItem(class APickUpItem* Item) override;
	virtual void ItemPickedUp(class APickUpItem* Item) override;

	void SetCurrentItem(APickUpItem * Item);

protected:
	UFUNCTION(NetMulticast, Reliable)
		void MULTI_NewItem(class APickUpItem* Item);

public:
	UPROPERTY(BlueprintReadWrite)
		class APickUpItem* CurrentItem;
protected:
};
