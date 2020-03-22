// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NetworkInitialiseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNetworkInitialiseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNDERFORGE_API INetworkInitialiseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
		void Initialise();

	UFUNCTION(BlueprintNativeEvent)
		void FullyInitialised();

	// If return true, will continue to initialise
	UFUNCTION(BlueprintNativeEvent)//BlueprintCallable, BlueprintImplementableEvent)//
		bool FinishedCreation();
	virtual bool FinishedCreation_Implementation();

};
