// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkController.h"

#include "NetworkSession/NWGameInstance.h"
#include "NetworkSession/NetworkGameMode.h"
#include "NetworkSession/NetworkPlayerState.h"
#include "NetworkSession/NetworkCharacter.h"
#include "NetworkSession/NetworkInitialiseComponent.h"
//#include "Game/ArenaGameInstance.h"

#include "Engine.h"

void ANetworkController::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast< ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("SERVER - Controller created ") + GetName());
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("CLIENT - Controller created ") + GetName());
	}
	ControllerCreated();

	UNWGameInstance* NetworkGI = Cast<UNWGameInstance>(GetGameInstance());
	if (IsLocalController()) NetworkGI->PassThroughController = this;
}

void ANetworkController::Destroyed()
{
	Super::Destroyed();
	if (GetGameInstance())
	{
		UNWGameInstance* NetworkGI = Cast<UNWGameInstance>(GetGameInstance());
		if (!NetworkGI) return;
		// If controller is this, set to null
		if (NetworkGI->PassThroughController == this)
		{
			NetworkGI->PassThroughController = nullptr;
		}
	}
}

void ANetworkController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	GameModeRef = Cast< ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Controller Post Seamless! - SERVER"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Controller Post Seamless! - CLIENT"));
	}
}

void ANetworkController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANetworkController::ServerTravelToMap(const FString MapName)
{
	const FString URL = "/Game/Maps/" + MapName + "?Listen";
	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, "Server traveling to map " + MapName);
	GetWorld()->ServerTravel(URL);
	//GetWorld()->SeamlessTravel(URL);
}

void ANetworkController::SeemlessTravelToNewMap(const FString MapName)
{
	const FString URL = "/Game/Maps/" + MapName + "?Listen";
	GetWorld()->SeamlessTravel(MapName);
}


void ANetworkController::ControllerCreated()
{
	if (GameModeRef) // Is server
	{
		ServerCreated = true;
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | SERVER TRUE!"));
		if (IsLocalPlayerController())
		{
			ClientCreated = true;
			if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | CLIENT TRUE!"));
		}
		GameModeRef->CheckControllersCreated();
	}
	else
	{
		SERVER_ControllerClientCreated();
	}
}

void ANetworkController::SERVER_ControllerClientCreated_Implementation()
{
	GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		ClientCreated = true;
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | CLIENT TRUE!"));
		GameModeRef->CheckControllersCreated();
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("ControllerClientCreated | GAME MODE NULL"));
	}
}
bool ANetworkController::SERVER_ControllerClientCreated_Validate()
{
	return true;
}

bool ANetworkController::AllCreated()
{
	return (ServerCreated && ClientCreated);
}

void ANetworkController::ControllerInitialised()
{
	if (GameModeRef) // Is server
	{
		ServerInitialised = true;
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | INIT SERVER TRUE!"));
		if (IsLocalPlayerController())
		{
			ClientInitialised = true;
			if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | INIT CLIENT TRUE!"));
		}
		if (AllInitialised())
		{
			FullyInitialised();
		}

		GameModeRef->CheckControllersInitialised();
	}
	else
	{
		SERVER_ControllerClientInitialised();
	}
}

bool ANetworkController::AllInitialised()
{
	return (ServerInitialised && ClientInitialised);
}

void ANetworkController::MULTI_PlayerReady_Implementation(ANetworkPlayerState* NetworkPlayerState)
{

}

void ANetworkController::SERVER_InitialiseComponentClientCreated_Implementation(UNetworkInitialiseComponent * NetworkInitialiseComponent)
{
	if (NetworkInitialiseComponent)
	{
		NetworkInitialiseComponent->ClientCreated(this);
	}
}
bool ANetworkController::SERVER_InitialiseComponentClientCreated_Validate(UNetworkInitialiseComponent * NetworkInitialiseComponent)
{
	return true;
}

void ANetworkController::SERVER_InitialiseComponentClientInitialised_Implementation(UNetworkInitialiseComponent * NetworkInitialiseComponent)
{
	NetworkInitialiseComponent->ClientInitialised(this);
}
bool ANetworkController::SERVER_InitialiseComponentClientInitialised_Validate(UNetworkInitialiseComponent* NetworkInitialiseComponent)
{
	return true;
}

void ANetworkController::MULTI_InitialiseComponentCreatedCheck_Implementation(UNetworkInitialiseComponent * NetworkInitialiseComponent)
{
	if (!IsLocalController()) return;
	if (NetworkInitialiseComponent)
	{
		NetworkInitialiseComponent->ObjectCreated();
	}
}

void ANetworkController::MULTI_InitialiseComponentInitialisedCheck_Implementation(UNetworkInitialiseComponent * NetworkInitialiseComponent)
{
	if (!IsLocalController()) return;
	if (NetworkInitialiseComponent)
	{
		NetworkInitialiseComponent->MULTI_Initialize_Implementation();
	}
}


void ANetworkController::SERVER_ControllerClientInitialised_Implementation()
{
	GameModeRef = Cast< ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
	{
		ClientInitialised = true;
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, GetName() + FString(" | INIT CLIENT TRUE!"));

		if (AllInitialised())
		{
			FullyInitialised();
		}
		GameModeRef->CheckControllersInitialised();
	}
	else
	{
		if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("ControllerClientInitialised | GAME MODE NULL"));
	}
}

bool ANetworkController::SERVER_ControllerClientInitialised_Validate()
{
	return true;
}

void ANetworkController::SERVER_PlayerStateClientCreated_Implementation(ANetworkPlayerState* NetworkPlayerState)
{
	if (NetworkPlayerState)
	{
		NetworkPlayerState->PlayerStateClientCreated(this);
	}
}
bool ANetworkController::SERVER_PlayerStateClientCreated_Validate(ANetworkPlayerState* NetworkPlayerState)
{
	return true;
}
void ANetworkController::SERVER_PlayerStateClientInitialised_Implementation(ANetworkPlayerState * NetworkPlayerState)
{
	NetworkPlayerState->PlayerStateClientInitialised(this);
}
bool ANetworkController::SERVER_PlayerStateClientInitialised_Validate(ANetworkPlayerState* NetworkPlayerState)
{
	return true;
}

//void ANetworkController::SERVER_PlayerPawnClientCreated_Implementation(ANetworkCharacter* NetworkPlayerPawn)
//{
//	if (NetworkPlayerPawn)
//	{
//		NetworkPlayerPawn->PlayerPawnClientCreated(this);
//	}
//}
//bool ANetworkController::SERVER_PlayerPawnClientCreated_Validate(ANetworkCharacter* NetworkPlayerPawn)
//{
//	return true;
//}
//void ANetworkController::SERVER_PlayerPawnClientInitialised_Implementation(ANetworkCharacter * NetworkPlayerPawn)
//{
//	NetworkPlayerPawn->PlayerPawnClientInitialised(this);
//}
//bool ANetworkController::SERVER_PlayerPawnClientInitialised_Validate(ANetworkCharacter* NetworkPlayerPawn)
//{
//	return true;
//}
//void ANetworkController::ReadyPlayerPawnClient_Implementation(ANetworkCharacter * NetworkCharacter)
//{
//	NetworkCharacter->ReadyPlayerPawnClient();
//}
//
//bool ANetworkController::ReadyPlayerPawnClient_Validate(ANetworkCharacter * NetworkCharacter)
//{
//	return true;
//}

void ANetworkController::Initialize_Implementation()
{
}

void ANetworkController::MULTI_FirstTimePlayerStateCreation_Implementation(int _PlayerNum)
{
	// Check player array has all players
	TArray<APlayerState*> PlayersArray = GetWorld()->GetGameState()->PlayerArray;
	if (PlayersArray.Num() < _PlayerNum)
	{
		// Not all players in player array, loop and check again in 1 sec
		FTimerHandle LoopCheck;
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &ANetworkController::MULTI_FirstTimePlayerStateCreation_Implementation, _PlayerNum);

		GetWorldTimerManager().SetTimer(LoopCheck, TimerDel, 1.0f, false);
		return;
	}

	// Only do once
	if (bFirstTimeCreationComplete) return;
	bFirstTimeCreationComplete = true;


	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Now readying player states " + FString::FormatAsNumber(PlayersArray.Num())));

	// Go through each player state and call NetworkSetup
	for (APlayerState* PState : PlayersArray)
	{
		if (ANetworkPlayerState* NetworkPState = Cast<ANetworkPlayerState>(PState))
		{
			NetworkPState->PlayerStateCreated();
			
		}
	}
}

void ANetworkController::MULTI_CreatedCheck_Implementation()
{
	ControllerCreated();
}

void ANetworkController::ResetInit()
{
	ServerCreated = false;
	ClientCreated = false;
}

void ANetworkController::InitializeComplete()
{
	ControllerInitialised();
}

void ANetworkController::FullyInitialised()
{
}

void ANetworkController::MULTI_Initialize_Implementation()
{
	// Already Initialised
	if (bHasBeenInitialised)
	{
		return;
	}

	// Not initialised, needs to
	bHasBeenInitialised = true;
	Initialize();
	InitializeComplete();
}

void ANetworkController::MULTI_PlayerStateCreatedCheck_Implementation(ANetworkPlayerState * NetworkPlayerState)
{
	if (!IsLocalController()) return;
	if (NetworkPlayerState)
	{
		NetworkPlayerState->PlayerStateCreated();
	}
}

void ANetworkController::MULTI_PlayerStateInitialisedCheck_Implementation(ANetworkPlayerState * NetworkPlayerState)
{
	if (!IsLocalController()) return;
	if (NetworkPlayerState)
	{
		NetworkPlayerState->MULTI_Initialize_Implementation();
	}
}

//void ANetworkController::MULTI_PlayerPawnCreatedCheck_Implementation(ANetworkCharacter * NetworkPlayerPawn)
//{
//	if (!IsLocalController()) return;
//	if (NetworkPlayerPawn)
//	{
//		NetworkPlayerPawn->PlayerPawnCreated();
//	}
//}
//
//void ANetworkController::MULTI_PlayerPawnInitialisedCheck_Implementation(ANetworkCharacter * NetworkPlayerPawn)
//{
//	if (!IsLocalController()) return;
//	if (NetworkPlayerPawn)
//	{
//		NetworkPlayerPawn->MULTI_Initialize_Implementation();
//	}
//}