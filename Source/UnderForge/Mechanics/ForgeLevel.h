// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ForgeLevel.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AForgeLevel : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TotalPartsCreated = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TotalWeaponsCreated = 0;

	UFUNCTION(BlueprintNativeEvent)
		void ForgeDestroyed();
};
