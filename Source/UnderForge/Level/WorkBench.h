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
	virtual void ProcessItem(class APickUpItem* Item) override;
	virtual void ItemPickedUp(class APickUpItem* Item) override;

protected:

public:
	class APickUpItem* CurrentItem;
protected:
};
