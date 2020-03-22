// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkInitialiseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERFORGE_API UNetworkInitialiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkInitialiseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game starts
	virtual void DestroyComponent(bool bPromoteChildren) override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Author: Alex
	// Called on begin play to communicate to network its been created on this client
	void ObjectCreated();

	// Author: Alex
	// Updates player controllers from network controllers array
	void S_UpdateControllersMap();

	// Author: Alex
	// Received from client to verify Network Initialise Component is created on said client, passes playcontroller sent through to know which client it came from
	void ClientCreated(class ANetworkController* PlayerControllerSent);
	// Author: Alex
	// Received from client to verify Network Initialise Component is initialised on said client, passes playcontroller sent through to know which client it came from
	void ClientInitialised(class ANetworkController* PlayerControllerSent);

	UFUNCTION(BlueprintPure)
		bool AllCreated();

	void Initialised();
	UFUNCTION(BlueprintPure)
		bool AllInitialised();

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

	// Must be called after initializing is complete in derived
	//UFUNCTION(BlueprintCallable)
	void InitializeComplete();

	// Author: Alex
	// Returns whether given controller has initialised this component
	bool S_CheckControllerInitialised(class ANetworkController* PlayerControllerSent);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UNWGameInstance* GameInstanceRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerPawnCreated)
		TMap<class ANetworkController*, bool> ClientsCreated;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerPawnCreated)
		TMap<class ANetworkController*, bool> ClientsInitialised;


	void SendOutChecks();
	FTimerHandle CheckHandle;
	float CheckDelay = 2.0f;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ANetworkGameMode* GameModeRef;

	bool bHasBeenInitialised = false;

	TArray<class ANetworkController*> UnconfirmedControllers;

	// TESTING
	UPROPERTY(EditDefaultsOnly)
	bool bDebugMessages = false;
};
