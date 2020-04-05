// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Incinerator.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AIncinerator : public AForgeStation
{
	GENERATED_BODY()
	
public:
	virtual bool CanTakeItem(class APickUpItem* Item) override;

	virtual void ProcessItem(class APickUpItem* Item) override;
};
