// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkSession/NetworkController.h"
#include "ForgeController.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AForgeController : public ANetworkController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void EndGame();

protected:
};
