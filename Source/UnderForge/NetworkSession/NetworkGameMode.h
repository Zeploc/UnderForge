// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetworkGameMode.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ANetworkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Author: Alex
	// Adds all network actors
	void AddAllNetworkFoundActors();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UNWGameInstance* NetworkGameInstance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<class ANetworkController*> NetworkPlayerControllers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class ANetworkCharacter> PlayerSpawnClass;

	// #### NETWORK INGAME SETUP ###

	virtual void PostSeamlessTravel() override;
	
	UFUNCTION(BlueprintCallable)
	// Author: Alex Coultas
	// Once all network entities initialised (PlayerState, PlayerController, but NOT PlayerPawn)
		virtual void AllInitialised();
	UFUNCTION(BlueprintImplementableEvent)
		void OnAllInitialised();

	UFUNCTION(BlueprintCallable)
	// Author: Alex Coultas
	// Once all Player Pawns are initialised
		virtual void AllPawnsInitialised();
	UFUNCTION(BlueprintImplementableEvent)
		void OnAllPawnsInitialised();
	

	UFUNCTION(BlueprintImplementableEvent)
		// Author: Alex Coultas
		// On Spawned Pawn created and initialised
		void SpawnedPawnInitialised(class ANetworkCharacter* InitialisedPawn);
	   	 

	UFUNCTION(BlueprintCallable)
	void CreatePlayers();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		// Author: Alex Coultas
		// Spawn player pawn for controller at location
	void SpawnPlayerPawn(class ANetworkController* NetworkController, FVector Location);

	UFUNCTION(BlueprintNativeEvent)
		// Author: Alex Coultas
		// On spawn player pawn
		void OnSpawnPlayerPawn(ANetworkCharacter* NetworkCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetNewPlayerSpawn();

	//UFUNCTION(BlueprintCallable)
	//void CheckIfAllReadyPawns();

	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void NetworkPlayerJoined(APlayerController* NewPlayer);
	virtual void Logout(AController * Exiting) override;
	virtual void NetworkPlayerLeft(AController* Exiting);
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

	// Author: Alex
	// Called when a player who was previously in the game and got disconnected, rejoins the game
	virtual void PlayerRejoined(ANetworkController* NetworkController);
	// Author: Alex
	// Called when a new player joins the game
	virtual void SpectatorJoined(ANetworkController* NetworkController);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool GameStarted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CreateSpawners = true;

	UPROPERTY(EditDefaultsOnly)
		float fStartRoundSpawnSelectionTime = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool ManuallyCreatingPlayers = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool ProgressiveJoining = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> CurrentNames;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UNetworkInitialiseComponent*> NetworkActors;
	TMap<class ANetworkController*, TArray<class UNetworkInitialiseComponent*>> ControllersComponentsReady;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<class UNetworkInitialiseComponent*> GetArrayFromNetworkController(class ANetworkController* Controller);

	void NetworkInitialiseComponentDestroyed(UNetworkInitialiseComponent* _NetworkInitialiseComponent);

	void ClearNetworkComponents();

	// Author: Alex
	// Once a network component is initialiesd, tells gamemode
	void NetworkActorFullyInitialised(class UNetworkInitialiseComponent* _NetworkComp, class ANetworkController* PlayerControllerSent);
	// Author: Alex
	// Check a player has fully initialised all network components/actors
	void CheckPlayerFullyReady(class ANetworkController* PlayerControllerSent);
	// Author: Alex
	// Once a player has fully initialised all network components/actors
	void PlayerFullyReady(class ANetworkController* PlayerControllerSent);
	// Author: Alex
	// Check all players fully ready
	void CheckAllPlayersFullyReady();

	// NEW
	UPROPERTY(BlueprintReadOnly)
	bool bControllersCreated = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerStatesCreated = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerPawnsCreated = false;

	void CheckControllersCreated();
	void CheckPlayerStatesCreated();
	void CheckPlayerPawnsCreated();

	void SendOutPlayerStateChecks();
	FTimerHandle PlayerStateCheckHandle;
	float PlayerStateCheckDelay = 2.0f;

	void SendOutControllerChecks();
	FTimerHandle ControllerCheckHandle;
	float ControllerCheckDelay = 2.0f;

	void SendOutPlayerPawnChecks();
	FTimerHandle PlayerPawnCheckHandle;
	float PlayerPawnCheckDelay = 2.0f;
	
	int iCurrentPawnCheckCount = 0;
	int iMaxPawnCheckCount = 5;

	void CheckControllersAndPlayerStatesCreated();
	void InitialisePlayerPawns();

	UPROPERTY(BlueprintReadOnly)
	bool bControllersInitialised = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerStatesInitialised = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerPawnsInitialised = false;

	void CheckControllersInitialised();
	void CheckPlayerStatesInitialised();
	void CheckPlayerPawnsInitialised();

	void CheckControllersAndPlayerStatesInitialised();

	void IndividualPlayerPawnsCheck();

	UFUNCTION(BlueprintCallable)
	void ResetControllersStatesSetup();
	UFUNCTION(BlueprintCallable)
	void ResetPawnSetup();

	// TESTING
	UPROPERTY(EditDefaultsOnly)
	bool bDebugMessages = true;
};
