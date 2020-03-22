// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NetworkSession/NetworkInitialiseInterface.h"
#include "NetworkCharacter.generated.h"

UCLASS()
class UNDERFORGE_API ANetworkCharacter : public ACharacter, public INetworkInitialiseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetworkCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UNetworkInitialiseComponent* NetworkInitialiseComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// INTERFACE
	UFUNCTION(BlueprintNativeEvent)
		void Initialise();
	virtual void Initialise_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void FullyInitialised();
	UFUNCTION(BlueprintNativeEvent)//BlueprintCallable, BlueprintImplementableEvent)//
		bool FinishedCreation();
	virtual bool FinishedCreation_Implementation() override;


	// Author: Alex
	// Overridable initialise event
	virtual void OnInitialize();

	// #### NETWORK INGAME SETUP ###

	//void NetworkSetup();
	//bool HasFullJoined();

	//void ReadyPlayerPawnClient();

	//UFUNCTION(Reliable, NetMulticast)
	//	void MultiInitialize();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//	void Initialize();


	//void ResetInit();

	//// Must be called after initializing is complete in derived
	////UFUNCTION(BlueprintCallable)
	//	void InitializeComplete();

	//virtual void PossessedBy(AController * NewController) override;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = NetworkSetup)
	//	bool ServerJoined = false;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = NetworkSetup)
	//	int ClientsJoined = 0;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class ANetworkGameMode* GameModeRef;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class UNWGameInstance* GameInstanceRef;

	//bool Initialised = false;


	// #### NETWORK INGAME SETUP ###

	//void PlayerPawnCreated();
	//UFUNCTION(BlueprintPure)
	//	bool AllCreated();

	//void PlayerPawnInitialised();
	//UFUNCTION(BlueprintPure)
	//	bool AllInitialised();

	//// Author: Alex
	//// Received from client to verify player Pawn created on said client, passes playcontroller sent through to know which client it came from
	//void PlayerPawnClientCreated(class ANetworkController* PlayerControllerSent);
	//// Author: Alex
	//// Received from client to verify player Pawn initialised on said client, passes playcontroller sent through to know which client it came from
	//void PlayerPawnClientInitialised(class ANetworkController* PlayerControllerSent);

	//UFUNCTION(BlueprintPure)
	//	// Author: Alex
	//	// Get number of clients created
	//	int GetClientsCreatedNumber();

	//UFUNCTION(BlueprintPure)
	//	// Author: Alex
	//	// Get number of clients Initialised
	//	int GetClientsInitialisedNumber();

	//// Author: Alex
	//// Sends out check to all non ready controllers
	//void S_CreatedCheck();

	//// Author: Alex
	//// Sends out check to all non ready controllers
	//void S_InitialisedCheck();

	//UFUNCTION(Reliable, NetMulticast)
	//	void MULTI_Initialize();

	//UFUNCTION(BlueprintNativeEvent)
	//	void Initialize();


	//// Must be called after initializing is complete in derived
	////UFUNCTION(BlueprintCallable)
	//void InitializeComplete();

	//// Author: Alex
	//// Updates player controllers from network controllers array
	//void S_UpdateControllersMap();

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerPawnCreated)
	//	TMap<class ANetworkController*, bool> ClientsCreated;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerPawnCreated)
	//	TMap<class ANetworkController*, bool> ClientsInitialised;

	//bool bHasBeenInitialised = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class ANetworkGameMode* GameModeRef;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class UNWGameInstance* GameInstanceRef;

	//// TESTING
	//bool bDebugMessages = false;
};
