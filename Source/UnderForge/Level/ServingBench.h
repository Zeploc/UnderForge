// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "ServingBench.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AServingBench : public AForgeStation
{
	GENERATED_BODY()
	
	//virtual void ItemDectection(class AActor* Item, bool entering) override;
	virtual bool CanTakeItem(class APickUpItem* Item) override;
	virtual void ProcessItem(class APickUpItem* Item) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void NewItem(AForgeItem* ForgeItem);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AForgeItem*> ForgeItems;
};
