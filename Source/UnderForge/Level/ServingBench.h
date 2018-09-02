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
	
	virtual void ItemDectection(class AActor* Item, bool entering) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AForgeItem*> ForgeItems;
};
