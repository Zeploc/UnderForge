// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"
#include "NetworkSession/NetworkGameMode.h"
#include "NetworkSession/NetworkController.h"
#include "NetworkSession/NWGameInstance.h"

#include "Engine/World.h"

void ANetworkPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	}
	GameInstanceRef = Cast<UNWGameInstance>(GetGameInstance());

	if (GameModeRef)
	{
		S_UpdateControllersMap();
	}
	PlayerStateCreated();
}

void ANetworkPlayerState::PlayerStateCreated()
{
	UNWGameInstance* NetworkGI = Cast<UNWGameInstance>(GetGameInstance());	
	APlayerController* LocalController = GetWorld()->GetFirstPlayerController();
	FString Controller = "No valid Controller!";
	if (ANetworkController* PassThroughController = Cast<ANetworkController>(NetworkGI->PassThroughController))
	{
		Controller = PassThroughController->GetName();			
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Created " + GetName() + " through " + Controller));
		if (HasAuthority())
		{
			// Set player state ready for local server controller
			PlayerStateClientCreated(PassThroughController);
		}
		else
		{
			// Pass through player state ready to server through local controller
			PassThroughController->SERVER_PlayerStateClientCreated(this);
		}
	}
	else
	{
		if (LocalController) Controller = LocalController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateCreated | CLIENT PASS THROUGH CONTROLLER NOt VALID, " + Controller));
	}
}
void ANetworkPlayerState::PlayerStateClientCreated(class ANetworkController* PlayerControllerSent)
{
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
			}
			else
			{
				// controller doesn't exist in network player controllers
				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateClientCreated | CONTROLLER SENT THROUGH ISIN'T IN PLAYER CONTROLLERS ARRAY"));
				return;
			}
		}
		else
		{
			// Controller exists in map, set to true
			ClientsCreated.Add(PlayerControllerSent, true);
		}
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" PlayerStateClientCreated | CLIENTS " + FString::FormatAsNumber(GetClientsCreatedNumber()) + " by controller " + PlayerControllerSent->GetName() + "!"));
		GameModeRef->CheckPlayerStatesCreated();
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateClientCreated | GAME MODE NULL"));
	}
}

bool ANetworkPlayerState::AllCreated()
{
	int PlayerNumber = 0;
	if (GameInstanceRef)
	{
		PlayerNumber = GameInstanceRef->HostNumberPlayers;
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerState AllCreated | GAME INSTANCE NULL"));
	}
	if (ClientsCreated.Num() < PlayerNumber)
	{
		// Not all controllers in map
		return false;
	}
	// If number of clients ready is equal to the map num, then all are ready
	return (GetClientsCreatedNumber() >= ClientsCreated.Num());
}

int ANetworkPlayerState::GetClientsCreatedNumber()
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
int ANetworkPlayerState::GetClientsInitialisedNumber()
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

void ANetworkPlayerState::S_CreatedCheck()
{
	S_UpdateControllersMap();
	TArray<ANetworkController*> ClientsControllers;
	ClientsCreated.GetKeys(ClientsControllers);
	// For each player controller
	for (ANetworkController* Controller : ClientsControllers)
	{
		// If controller has not verified creation of this player state
		if (!ClientsCreated[Controller])
		{
			// Send to client to verify creation again
			Controller->MULTI_PlayerStateCreatedCheck(this);
		}
	}
}

void ANetworkPlayerState::S_InitialisedCheck()
{
	TArray<ANetworkController*> ClientsControllers;
	ClientsInitialised.GetKeys(ClientsControllers);
	// For each player controller
	for (ANetworkController* Controller : ClientsControllers)
	{
		// If controller has not verified creation of this player state
		if (!ClientsInitialised[Controller])
		{
			// Send to client to verify creation again
			Controller->MULTI_PlayerStateInitialisedCheck(this);
		}
	}
}

void ANetworkPlayerState::PlayerStateInitialised()
{
	UNWGameInstance* NetworkGI = Cast<UNWGameInstance>(GetGameInstance());
	ANetworkController* PassThroughController = NetworkGI->PassThroughController;// Cast<ANetworkController>(GetWorld()->GetFirstPlayerController());
	FString Controller = "No valid Controller!";
	if (PassThroughController)
	{
		Controller = PassThroughController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Client Initialised on Player State through " + Controller));
		if (HasAuthority())
		{
			// Set player state ready for local server controller
			PlayerStateClientInitialised(PassThroughController);
		}
		else
		{
			// Pass through player state ready to server through local controller
			PassThroughController->SERVER_PlayerStateClientInitialised(this);
		}
	}
	else
	{
		if (PassThroughController) Controller = PassThroughController->GetName();
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateInitialised | CLIENT PASS THROUGH CONTROLLER NOt VALID, " + Controller));
	}
}

void ANetworkPlayerState::PlayerStateClientInitialised(class ANetworkController* PlayerControllerSent)
{
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
				if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateClientCreated | CONTROLLER SENT THROUGH ISIN'T IN PLAYER CONTROLLERS ARRAY"));
				return;
			}
		}
		else
		{
			// Controller exists in map, set to true
			ClientsInitialised.Add(PlayerControllerSent, true);
		}
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | INIT CLIENTS " + FString::FormatAsNumber(GetClientsCreatedNumber()) + " by controller " + PlayerControllerSent->GetName() + "!"));
		
		if (AllInitialised())
		{
			FullyInitialised();
		}
		GameModeRef->CheckPlayerStatesInitialised();
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerStateClientInitialised | GAME MODE NULL"));
	}
}


bool ANetworkPlayerState::AllInitialised()
{
	int PlayerNumber = 0;
	if (GameInstanceRef)
	{
		PlayerNumber = GameInstanceRef->HostNumberPlayers;
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("PlayerState AllInitialised | GAME INSTANCE NULL"));
	}
	if (ClientsInitialised.Num() < PlayerNumber)
	{
		// Not all controllers in map
		return false;
	}
	// If number of clients ready is equal to the map num, then all are ready
	return (GetClientsInitialisedNumber() >= ClientsInitialised.Num());
}


void ANetworkPlayerState::Initialize_Implementation()
{
}

void ANetworkPlayerState::InitializeComplete()
{
	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("InitializeComplete | CLIENT INITIALISE COMPLETE"));

	PlayerStateInitialised();
}

void ANetworkPlayerState::FullyInitialised()
{
}

void ANetworkPlayerState::S_UpdateControllersMap()
{
	if (!GameModeRef)
	{
		GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	}

	if (GameModeRef)
	{
		for (ANetworkController* Controller : GameModeRef->NetworkPlayerControllers)
		{
			if (!ClientsCreated.Contains(Controller))
			{
				ClientsCreated.Add(Controller, false);
			}
			if (!ClientsInitialised.Contains(Controller))
			{
				ClientsInitialised.Add(Controller, false);
			}
		}
	}
}

void ANetworkPlayerState::MULTI_Initialize_Implementation()
{
	if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("MULTI_Initialize | CLIENT INITIALISE"));

	// Already Initialised
	if (bHasBeenInitialised)
	{
		PlayerStateInitialised();
		return;
	}

	// Not initialised, needs to
	bHasBeenInitialised = true;
	Initialize();
	InitializeComplete();
}
