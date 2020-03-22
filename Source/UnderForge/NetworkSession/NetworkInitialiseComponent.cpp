// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkInitialiseComponent.h"

#include "NetworkSession/NWGameInstance.h"
#include "NetworkSession/NetworkController.h"
#include "NetworkSession/NetworkGameMode.h"
#include "NetworkSession/NetworkInitialiseInterface.h"

#include "Public/TimerManager.h"
#include "Engine/World.h"
//#include "Engine.h"

// Sets default values for this component's properties
UNetworkInitialiseComponent::UNetworkInitialiseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNetworkInitialiseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
		GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GetOwner())
		GameInstanceRef = Cast<UNWGameInstance>(GetOwner()->GetGameInstance());
	
	if (GameModeRef)
	{
		S_UpdateControllersMap();
		GameModeRef->NetworkActors.AddUnique(this);
	}
	ObjectCreated();
}

void UNetworkInitialiseComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);

	if (GetOwner())
	{
		if (!GetOwner()->HasAuthority())
		{
			return;
		}
	}
	
	if (!GameModeRef)
		GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		GameModeRef->NetworkInitialiseComponentDestroyed(this);
	}

	if (!GameModeRef)
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, GetOwner()->GetName() + FString(" DestroyComponent | Can't get gamemode for destroy"));
	}

	// Clear Timer
	GetOwner()->GetWorldTimerManager().ClearTimer(CheckHandle);

}

void UNetworkInitialiseComponent::ObjectCreated()
{
	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent ObjectCreated | " + GetOwner()->GetName() + " Running"));

	// If game instance not valid, attempt to get it
	if (!GameInstanceRef)
	{
		GameInstanceRef = Cast<UNWGameInstance>(GetOwner()->GetGameInstance());
	}
	if (!GameInstanceRef)
	{
		// Can't get game instance
		return;
	}

	APlayerController* LocalController = GetWorld()->GetFirstPlayerController();
	FString _Controller = "No valid Controller!";
	if (ANetworkController* PassThroughController = Cast<ANetworkController>(GameInstanceRef->PassThroughController))
	{
		_Controller = PassThroughController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent Created " + GetOwner()->GetName() + " through " + _Controller));
		if (GetOwner()->HasAuthority())
		{
			// Set player Pawn ready for local server controller
			ClientCreated(PassThroughController);
		}
		else
		{
			// Pass through player Pawn ready to server through local controller
			PassThroughController->SERVER_InitialiseComponentClientCreated(this);
		}
	}
	else
	{
		if (LocalController) _Controller = LocalController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent ObjectCreated | CLIENT PASS THROUGH CONTROLLER NOT VALID, " + _Controller));
	}

	// Start delay time for created check
	GetOwner()->GetWorldTimerManager().SetTimer(CheckHandle, this, &UNetworkInitialiseComponent::SendOutChecks, CheckDelay, false);
}

void UNetworkInitialiseComponent::S_UpdateControllersMap()
{
	if (!GetWorld())
	{
		return;
	}
	if (!GameModeRef)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AGameModeBase* Gamemode = World->GetAuthGameMode();
			if (Gamemode)
			{
				GameModeRef = Cast<ANetworkGameMode>(Gamemode);
			}
		}
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent S_UpdateControllersMap | Could not get gamemode"));

	}

	if (GameModeRef)
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent S_UpdateControllersMap | Updating " + FString::FormatAsNumber(GameModeRef->NetworkPlayerControllers.Num()) + " Controllers from gamemode"));
		for (ANetworkController* _Controller : GameModeRef->NetworkPlayerControllers)
		{
			if (!ClientsCreated.Contains(_Controller))
			{
				ClientsCreated.Add(_Controller, false);
				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent S_UpdateControllersMap | Adding Controller " + _Controller->GetName() + " to clients created"));
			}
			if (!ClientsInitialised.Contains(_Controller))
			{
				ClientsInitialised.Add(_Controller, false);
			}
		}
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent S_UpdateControllersMap | Could not get gamemode"));
	}

}

// Called every frame
void UNetworkInitialiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	TArray<ANetworkController*> Controllers;
	ClientsInitialised.GetKeys(Controllers);
	
	for (ANetworkController* Controller : Controllers)
	{
		if (!GetOwner())
			continue;
		//FString Message = GetOwner()->GetName() + "Status for " + Controller->GetName() + " is " + FString(ClientsInitialised[Controller] ? "Ready" : "Not ready");
		//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Cyan, Message);
	}

}

void UNetworkInitialiseComponent::ClientCreated(ANetworkController * PlayerControllerSent)
{
	if (GetWorld())
		GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		// Checks to see if sent through controller is not in current map
		if (!ClientsCreated.Contains(PlayerControllerSent))
		{
			// Checks if the sent controller is a controller not yet added to map
			if (GameModeRef->NetworkPlayerControllers.Contains(PlayerControllerSent))
			{
				// Addeds to controller check map
				ClientsCreated.Add(PlayerControllerSent, true);
				ClientsInitialised.Add(PlayerControllerSent, false);

				// If controller was in unconfirmed, means it was readded, so remove it
				if (UnconfirmedControllers.Contains(PlayerControllerSent))
				{
					UnconfirmedControllers.Remove(PlayerControllerSent);
				}
				
			}
			else
			{
				UnconfirmedControllers.Add(PlayerControllerSent);
				// controller doesn't exist in network player controllers
				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent ClientCreated | CONTROLLER SENT THROUGH ISIN'T IN PLAYER CONTROLLERS ARRAY"));
				return;
			}
		}
		else
		{
			// Controller exists in map, set to true
			ClientsCreated.Add(PlayerControllerSent, true);
		}
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetOwner()->GetName() + FString(" ClientCreated | CLIENTS " + FString::FormatAsNumber(GetClientsCreatedNumber()) + " by controller " + PlayerControllerSent->GetName() + "!"));
		//GameModeRef->CheckPlayerPawnsCreated();

		// If fully created (all clients)
		if (AllCreated())
		{
			// if not initialised and has not been initialised
			if (!AllInitialised() && !bHasBeenInitialised)
			{
				bool bInitialise = true;
				// If has interface
				if (GetOwner()->GetClass()->ImplementsInterface(UNetworkInitialiseInterface::StaticClass()))
				{
					// If initialise should not continue through
					if (!INetworkInitialiseInterface::Execute_FinishedCreation(GetOwner()))
					{
						// Do not initialise
						bInitialise = false;
					}
					//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString("CALL INITIALISE"));
				}

				if (bInitialise)
				{
					// Initialise for all clients
					MULTI_Initialize();

					// Start delay time for initialise check
					GetOwner()->GetWorldTimerManager().SetTimer(CheckHandle, this, &UNetworkInitialiseComponent::SendOutChecks, CheckDelay, false);
				}
			}
		}
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent ClientCreated | GAME MODE NULL"));
	}
}

bool UNetworkInitialiseComponent::AllCreated()
{
	int PlayerNumber = 0;
	if (GameInstanceRef)
	{
		PlayerNumber = GameInstanceRef->HostNumberPlayers;
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent AllCreated | GAME INSTANCE NULL"));
	}
	if (ClientsCreated.Num() < PlayerNumber)
	{
		// Not all controllers in map
		return false;
	}
	// If number of clients ready is equal to the map num, then all are ready
	return (GetClientsCreatedNumber() >= ClientsCreated.Num());
}

int UNetworkInitialiseComponent::GetClientsCreatedNumber()
{
	TArray<bool> ClientsCreatedBools;
	ClientsCreated.GenerateValueArray(ClientsCreatedBools);
	int ClientsCreatedNumber = 0;
	for (bool Created : ClientsCreatedBools)
	{
		if (Created) ClientsCreatedNumber++;
	}
	return ClientsCreatedNumber;
}
int UNetworkInitialiseComponent::GetClientsInitialisedNumber()
{
	TArray<bool> ClientsCreatedBools;
	ClientsInitialised.GenerateValueArray(ClientsCreatedBools);
	int ClientsInitialisedNumber = 0;
	for (bool Initialised : ClientsCreatedBools)
	{
		if (Initialised) ClientsInitialisedNumber++;
	}
	return ClientsInitialisedNumber;
}


void UNetworkInitialiseComponent::S_CreatedCheck()
{
	if (GetOwner())
	{
		if (!GetOwner()->HasAuthority())
			return;
	}
	S_UpdateControllersMap();
	TArray<ANetworkController*> ClientsControllers;
	ClientsCreated.GetKeys(ClientsControllers);

	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent S_CreatedCheck | Controllers sending out created check to " + FString::FormatAsNumber(ClientsControllers.Num()) + " number of controllers"));

	// For each player controller
	for (ANetworkController* _Controller : ClientsControllers)
	{
		// If controller has not verified creation of this player Pawn
		if (ClientsCreated.Contains(_Controller))
		{
			if (!ClientsCreated[_Controller])
			{
				// Send to client to verify creation again
				_Controller->MULTI_InitialiseComponentCreatedCheck(this);

				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitialiseComponent S_CreatedCheck | Sending Creation check to " + _Controller->GetName()));
			}
			
		}
		else
		{
			if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent S_CreatedCheck | Controller " + _Controller->GetName() + " not in clients created"));

		}
	}
}


void UNetworkInitialiseComponent::S_InitialisedCheck()
{
	TArray<ANetworkController*> ClientsControllers;
	ClientsInitialised.GetKeys(ClientsControllers);

	if (ClientsControllers.Num() <= 0)
		return;

	// For each player controller
	for (ANetworkController* _Controller : ClientsControllers)
	{
		if (!ClientsInitialised.Contains(_Controller))
			continue;
		// If controller has not verified creation of this player Pawn
		if (!ClientsInitialised[_Controller])
		{
			if (INetworkInitialiseInterface::Execute_FinishedCreation(GetOwner()))
			{
				// Send to client to verify creation again
				_Controller->MULTI_InitialiseComponentInitialisedCheck(this);
			}
		}
	}
}

void UNetworkInitialiseComponent::Initialised()
{
	UNWGameInstance* NetworkGI = Cast<UNWGameInstance>(GetOwner()->GetGameInstance());
	if (!NetworkGI)
	{
		//if (bDebugMessages)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("Client NOT Initialised, NO GAME INSTANCE"));
		return;
	}

	ANetworkController* PassThroughController = NetworkGI->PassThroughController;// Cast<ANetworkController>(GetWorld()->GetFirstPlayerController());
	FString _Controller = "No valid Controller!";
	if (PassThroughController)
	{
		_Controller = PassThroughController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetOwner()->GetName() + FString(" INITIALISED | Client Initialised through " + _Controller));       
		if (GetOwner()->HasAuthority())
		{
			// Set player Pawn ready for local server controller
			ClientInitialised(PassThroughController);
		}
		else
		{
			// Pass through player Pawn ready to server through local controller
			PassThroughController->SERVER_InitialiseComponentClientInitialised(this);
		}
	}
	else
	{
		if (PassThroughController) _Controller = PassThroughController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerPawnInitialised | CLIENT PASS THROUGH CONTROLLER NOt VALID, " + _Controller));
	}
}

void UNetworkInitialiseComponent::ClientInitialised(class ANetworkController* PlayerControllerSent)
{
	if (!GetWorld())
		return;
	GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		// Checks to see if sent through controller is not in current map
		if (!ClientsInitialised.Contains(PlayerControllerSent))
		{
			// Checks if the sent controller is a controller not yet added to map
			if (GameModeRef->NetworkPlayerControllers.Contains(PlayerControllerSent))
			{
				// Addeds to controller check map
				ClientsInitialised.Add(PlayerControllerSent, true);
			}
			else
			{
				// controller doesn't exist in network player controllers
				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("ClientInitialised | CONTROLLER SENT THROUGH ISIN'T IN PLAYER CONTROLLERS ARRAY"));
				return;
			}
		}
		else
		{
			// Controller exists in map, set to true
			ClientsInitialised.Add(PlayerControllerSent, true);
		}
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetOwner()->GetName() + FString(" ClientInitialised | CLIENTS " + FString::FormatAsNumber(GetClientsInitialisedNumber()) + " by controller " + PlayerControllerSent->GetName() + "!"));

		//GameModeRef->CheckPlayerPawnsInitialised();
		
		if (AllInitialised())
		{
			// On all initialised through interface
			if (GetOwner()->GetClass()->ImplementsInterface(UNetworkInitialiseInterface::StaticClass()))
			{
				INetworkInitialiseInterface::Execute_FullyInitialised(GetOwner());
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString("Fully INITIALISE"));
			}


			// Initialise and created, stop timer
			GetOwner()->GetWorldTimerManager().ClearTimer(CheckHandle);
		}

		GameModeRef->NetworkActorFullyInitialised(this, PlayerControllerSent);
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("ClientInitialised | GAME MODE NULL"));
	}
}


bool UNetworkInitialiseComponent::AllInitialised()
{
	int PlayerNumber = 0;
	if (GameInstanceRef)
	{
		PlayerNumber = GameInstanceRef->HostNumberPlayers;
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerPawn AllInitialised | GAME INSTANCE NULL"));
	}
	if (ClientsInitialised.Num() < PlayerNumber)
	{
		// Not all controllers in map
		return false;
	}
	// If number of clients ready is equal to the map num, then all are ready
	return (GetClientsInitialisedNumber() >= ClientsInitialised.Num());
}



void UNetworkInitialiseComponent::InitializeComplete()
{
	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetOwner()->GetName() + FString(" InitializeComplete | CLIENT INITIALISE COMPLETE"));

	Initialised();
}

bool UNetworkInitialiseComponent::S_CheckControllerInitialised(ANetworkController * PlayerControllerSent)
{
	if (!ClientsInitialised.Contains(PlayerControllerSent))
		return false;

	return ClientsInitialised[PlayerControllerSent];
}

void UNetworkInitialiseComponent::SendOutChecks()
{
	if (!GetOwner())
		return;

	if (!AllCreated())
	{
		// Not created yet

		// Send check to clients not created
		S_CreatedCheck();
	}
	else if (!AllInitialised())
	{
		// Not initialised yet

		// Send check to clients not created
		S_InitialisedCheck();
	}
	else
	{
		// Initialise and created, stop timer
		GetOwner()->GetWorldTimerManager().ClearTimer(CheckHandle);
		return;
	}

	//if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("InitialiseComponent SendOutChecks | Check Failed, Sending again in " + FString::SanitizeFloat(CheckDelay) + " for " + GetOwner()->GetName()));

	// Check again in delay time, will get canceled if all are ready
	GetOwner()->GetWorldTimerManager().SetTimer(CheckHandle, this, &UNetworkInitialiseComponent::SendOutChecks, CheckDelay, false);
}

void UNetworkInitialiseComponent::MULTI_Initialize_Implementation()
{
	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetOwner()->GetName() + FString(" MULTI_Initialize | CLIENT INITIALISE"));

	// Already Initialised
	if (bHasBeenInitialised)
	{
		Initialised();
		return;
	}

	// Not initialised, needs to
	bHasBeenInitialised = true;
	//OnInitialize();

	if (GetOwner()->GetClass()->ImplementsInterface(UNetworkInitialiseInterface::StaticClass()))
	{
		INetworkInitialiseInterface::Execute_Initialise(GetOwner());
		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString("CALL INITIALISE"));
	}


	InitializeComplete();
}
