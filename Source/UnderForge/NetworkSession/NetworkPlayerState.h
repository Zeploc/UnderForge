// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetworkPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API ANetworkPlayerState : public APlayerState
{
	GENERATED_BODY()


public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// #### NETWORK INGAME SETUP ###

	void PlayerStateCreated();
	UFUNCTION(BlueprintPure)
	bool AllCreated();

	void PlayerStateInitialised();
	UFUNCTION(BlueprintPure)
	bool AllInitialised();
	
	// Author: Alex
	// Received from client to verify player state created on said client, passes playcontroller sent through to know which client it came from
	void PlayerStateClientCreated(class ANetworkController* PlayerControllerSent);
	// Author: Alex
	// Received from client to verify player state initialised on said client, passes playcontroller sent through to know which client it came from
	void PlayerStateClientInitialised(class ANetworkController* PlayerControllerSent);

	UFUNCTION(BlueprintPure)
	// Author: Alex
	// Get number of clients created
	int GetClientsCreatedNumber();

	UFUNCTION(BlueprintPure)
		// Author: Alex
		// Get number of clients Initialised
	int GetClientsInitialisedNumber();

	// Author: Alex
	// Sends out check to all non ready controllers
	void S_CreatedCheck();

	// Author: Alex
	// Sends out check to all non ready controllers
	void S_InitialisedCheck();

	UFUNCTION(Reliable, NetMulticast)
		void MULTI_Initialize();

	UFUNCTION(BlueprintNativeEvent)
		void Initialize();


	void InitializeComplete();

	virtual void FullyInitialised();
	
	// Author: Alex
	// Updates player controllers from network controllers array
	void S_UpdateControllersMap();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerStateCreated)
		TMap<class ANetworkController*, bool> ClientsCreated;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerStateCreated)
		TMap<class ANetworkController*, bool> ClientsInitialised;

	bool bHasBeenInitialised = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ANetworkGameMode* GameModeRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UNWGameInstance* GameInstanceRef;

	// TESTING
	UPROPERTY(EditDefaultsOnly)
	bool bDebugMessages = false;
};
