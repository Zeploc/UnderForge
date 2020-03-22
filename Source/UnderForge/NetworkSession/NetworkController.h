// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Online.h"
#include "NetworkController.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ANetworkController : public APlayerController
{
	GENERATED_BODY()

protected:

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void PostSeamlessTravel() override;

	// #### HOSTING / JOINING ####		

	UFUNCTION(BlueprintCallable)
		void ServerTravelToMap(const FString MapName = "BasicLevel");

	UFUNCTION(BlueprintCallable)
		void SeemlessTravelToNewMap(const FString MapName = "Olympus_1");

	
	// #### NETWORK INGAME SETUP ###

	void ControllerCreated();
	bool AllCreated();
	void ControllerInitialised();
	bool AllInitialised();

	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_ControllerClientCreated();
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_ControllerClientInitialised();
	

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// Tells player that a specific player is ready
		void MULTI_PlayerReady(class ANetworkPlayerState* NetworkPlayerState);
	virtual void MULTI_PlayerReady_Implementation(class ANetworkPlayerState* NetworkPlayerState);

	// NetworkInitialiseComponent

	UFUNCTION(Reliable, Server, WithValidation)
		// Author: Alex
		// Received from client to verify a initialise component created on said client
		void SERVER_InitialiseComponentClientCreated(class UNetworkInitialiseComponent* NetworkInitialiseComponent);

	UFUNCTION(Reliable, Server, WithValidation)
		// Author: Alex
		// Received from client to verify a initialise component initialised on said client
		void SERVER_InitialiseComponentClientInitialised(class UNetworkInitialiseComponent* NetworkInitialiseComponent);

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// Sends to client to check if a initialise component is created
		void MULTI_InitialiseComponentCreatedCheck(class UNetworkInitialiseComponent* NetworkInitialiseComponent);

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// Sends to client to check if a initialise component is initialised
		void MULTI_InitialiseComponentInitialisedCheck(class UNetworkInitialiseComponent* NetworkInitialiseComponent);




	// PLAYER STATE
	UFUNCTION(Reliable, Server, WithValidation)
		// Author: Alex
		// Received from client to verify player state created on said client
		void SERVER_PlayerStateClientCreated(class ANetworkPlayerState* NetworkPlayerState);


	UFUNCTION(Reliable, Server, WithValidation)
		// Author: Alex
		// Received from client to verify player state initialised on said client
		void SERVER_PlayerStateClientInitialised(class ANetworkPlayerState* NetworkPlayerState);

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// Sends to client to check if player state is created
	void MULTI_PlayerStateCreatedCheck(class ANetworkPlayerState* NetworkPlayerState);

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// Sends to client to check if player state is initialised
	void MULTI_PlayerStateInitialisedCheck(class ANetworkPlayerState* NetworkPlayerState);


	// PLAYER PAWN
	//UFUNCTION(Reliable, Server, WithValidation)
	//	// Author: Alex
	//	// Received from client to verify player Pawn initialised on said client
	//	void SERVER_PlayerPawnClientInitialised(class ANetworkCharacter* NetworkPlayerPawn);

	//UFUNCTION(Reliable, NetMulticast)
	//	// Author: Alex
	//	// Sends to client to check if player Pawn is created
	//	void MULTI_PlayerPawnCreatedCheck(class ANetworkCharacter* NetworkPlayerPawn);

	//UFUNCTION(Reliable, NetMulticast)
	//	// Author: Alex
	//	// Sends to client to check if player Pawn is initialised
	//	void MULTI_PlayerPawnInitialisedCheck(class ANetworkCharacter* NetworkPlayerPawn);

	//UFUNCTION(Reliable, Server, WithValidation)
	//	// Author: Alex
	//	// Received from client to verify player Pawn created on said client
	//	void SERVER_PlayerPawnClientCreated(class ANetworkCharacter* NetworkPlayerPawn);


	//UFUNCTION(Reliable, Server, WithValidation)
	//	void ReadyPlayerPawnClient(class ANetworkCharacter* NetworkCharacter);

	UFUNCTION(Reliable, NetMulticast)
		void MULTI_Initialize();

	UFUNCTION(Reliable, NetMulticast)
		// Author: Alex
		// First time initialise player states
		void MULTI_FirstTimePlayerStateCreation(int _PlayerNum);

	UFUNCTION(NetMulticast, Reliable)
	// Author: Alex
	// Sends out check to all non ready controllers
	void MULTI_CreatedCheck();


	UFUNCTION(BlueprintNativeEvent)
		void Initialize();
	virtual void Initialize_Implementation();

	void ResetInit();

	void InitializeComplete();

	virtual void FullyInitialised();


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ControllerCreated)
		bool ServerCreated = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ControllerCreated)
		bool ClientCreated = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ControllerCreated)
		bool ServerInitialised = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ControllerCreated)
		bool ClientInitialised = false;

	bool bHasBeenInitialised = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ANetworkGameMode* GameModeRef;

	bool bFirstTimeCreationComplete = false;

	// TESTING
	UPROPERTY(EditDefaultsOnly)
	bool bDebugMessages = false;
};
