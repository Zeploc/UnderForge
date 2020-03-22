// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"

#include "NetworkSession/NWGameInstance.h"
#include "NetworkSession/NetworkController.h"
#include "NetworkSession/NetworkPlayerState.h"
#include "NetworkSession/NetworkCharacter.h"
#include "NetworkSession/NetworkInitialiseComponent.h"

//#include "Player/ArenaMultiplayerPC.h"
#include "CommonFunctions.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine.h"

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();

	NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	UCommonFunctions::AddDebugMessage(FString("Gamemode created!"), false, bDebugMessages);

	GetWorldTimerManager().SetTimer(PlayerStateCheckHandle, this, &ANetworkGameMode::SendOutPlayerStateChecks, PlayerStateCheckDelay, false);
	GetWorldTimerManager().SetTimer(ControllerCheckHandle, this, &ANetworkGameMode::SendOutControllerChecks, ControllerCheckDelay, false);

	AddAllNetworkFoundActors();

}

void ANetworkGameMode::AddAllNetworkFoundActors()
{
	// Find all actors with interface
	TArray<AActor*> NetworkInterfaceActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UNetworkInitialiseInterface::StaticClass(), NetworkInterfaceActors);
	for (AActor* FoundActor : NetworkInterfaceActors)
	{
		// For each one add init comp if not added to array
		UNetworkInitialiseComponent* FoundInitComp = Cast< UNetworkInitialiseComponent>(FoundActor->GetComponentByClass(UNetworkInitialiseComponent::StaticClass()));
		NetworkActors.AddUnique(FoundInitComp);

		// Check if it has any unconfirmed controllers
		if (FoundInitComp->UnconfirmedControllers.Num() > 0)
		{
			for (ANetworkController* Controller : FoundInitComp->UnconfirmedControllers)
			{
				// For all unconfirmed, check if they are now in controllers array
				if (NetworkPlayerControllers.Contains(Controller))
				{
					// Controller is now valid, so redo creation
					FoundInitComp->ClientCreated(Controller);
				}
			}
		}
	}
}

void ANetworkGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDebugMessages)
	{
		TArray<ANetworkController*> Controllers;
		TArray<TArray<UNetworkInitialiseComponent*>> NetworkComps;
		ControllersComponentsReady.GetKeys(Controllers);
		ControllersComponentsReady.GenerateValueArray(NetworkComps);
		for (int i = 0; i < Controllers.Num(); i++)
		{
			int CompsReady = NetworkComps[i].Num();
			FString Message = Controllers[i]->GetName() + " has " + FString::FormatAsNumber(CompsReady) + " / " + FString::FormatAsNumber(NetworkActors.Num()) + " Ready";
			if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Black, Message);
		}
		for (int i = 0; i < NetworkActors.Num(); i++)
		{
			FString Message = FString::FormatAsNumber(i) + ": ";
			UNetworkInitialiseComponent* Comp = NetworkActors[i];
			if (Comp)
			{
				AActor* CompOwner = Comp->GetOwner();
				if (CompOwner)
				{
					ensure(CompOwner != NULL);
					Message += CompOwner->GetName();
				}
			}
			if (bDebugMessages) GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Black, Message);
		}
	}


	int Size = NetworkActors.Num();
	for (int i = 0; i < Size; i++)
	{
		UNetworkInitialiseComponent* Comp = NetworkActors[i];
		if (!Comp)
		{
			NetworkActors.RemoveAt(i);
			i = -1;
			Size = NetworkActors.Num();
			// Need check since change?
		}
	}
	/*TArray<class UNetworkInitialiseComponent*> NetworkActors;
	ControllersComponentsReady.GenerateValueArray()*/
}

void ANetworkGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Post Seemless Travel! with GM: ") + GetName());
}

void ANetworkGameMode::AllInitialised()
{
	OnAllInitialised();
}

void ANetworkGameMode::AllPawnsInitialised()
{
	if (!ProgressiveJoining)
	{
		GameStarted = true;
	}
}

void ANetworkGameMode::CreatePlayers()
{
	ResetPawnSetup();
	TArray<AActor*> ExistingCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetworkCharacter::StaticClass(), ExistingCharacters);
	for (int i = 0; i < ExistingCharacters.Num(); i++)
	{
		ExistingCharacters[i]->Destroy();
	}

	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Spawn the players
	{
		/*AArenaMultiplayerPC* ArenaPC = Cast<AArenaMultiplayerPC>(NetworkPlayerControllers[i]);
		if (ArenaPC && CreateSpawners)
		{
			ArenaPC->StartRespawnCycle(fStartRoundSpawnSelectionTime);
		}
		else
		{*/
			FVector SpawnLocation = GetNewPlayerSpawn();
			if (NetworkPlayerControllers[i]->GetPawn())
				SpawnLocation = NetworkPlayerControllers[i]->GetPawn()->GetActorLocation();
			SpawnPlayerPawn(NetworkPlayerControllers[i], SpawnLocation);
		//}
	}

}

void ANetworkGameMode::SpawnPlayerPawn_Implementation(ANetworkController * NetworkController, FVector Location)
{
	if (APawn* Pawn = NetworkController->GetPawn())
	{
		Pawn->Destroy();
	}

	FVector SpawnLocation = Location;// GetNewPlayerSpawn()

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ANetworkCharacter* NetworkCharacter = GetWorld()->SpawnActor<ANetworkCharacter>(PlayerSpawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (NetworkCharacter)
	{
		NetworkController->Possess(NetworkCharacter);
		CheckPlayerPawnsCreated();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("CREATING PLAYER for controller ") + NetworkController->GetName() + FString(" called ") + NetworkCharacter->GetName());
	}
	else
	{

	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("COUDLN'T CREATE PLAYER for controller ") + NetworkController->GetName());
	OnSpawnPlayerPawn(NetworkCharacter);
}

void ANetworkGameMode::OnSpawnPlayerPawn_Implementation(ANetworkCharacter* NetworkCharacter)
{


}

void ANetworkGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NetworkPlayerJoined(NewPlayer);
}

void ANetworkGameMode::NetworkPlayerJoined(APlayerController* NewPlayer)
{
	ANetworkController* NetworkController = Cast<ANetworkController>(NewPlayer);
	bool JoinInProgress = false;
	if (NetworkGameInstance)
	{
		JoinInProgress = NetworkGameInstance->bJoinInProgress;
	}
	if (GameStarted && !ProgressiveJoining && !JoinInProgress)
	{
		bool OriginalPlayer = false;
		if (NetworkController->PlayerState)
		{
			if (CurrentNames.Contains(NetworkController->PlayerState->GetPlayerName()))
			{
				OriginalPlayer = true;
			}
		}

		if (!OriginalPlayer) // not original player, don't add to controllers
		{
			/// Most likely skipped since not using controlelr initialise system
			SpectatorJoined(NetworkController);
			return;
		}
	}

	if (NetworkController) // Player Joins (Needs lock for during game)
	{
		NetworkPlayerControllers.Add(NetworkController);
		if (!ControllersComponentsReady.Contains(NetworkController))
		{
			ControllersComponentsReady.Add(NetworkController, TArray<UNetworkInitialiseComponent*>());
		}
		// For all existing comps of new controller
		for (UNetworkInitialiseComponent* FoundComp : NetworkActors)
		{
			if (!FoundComp)
				continue;

			// Check if it has any unconfirmed controllers
			if (FoundComp->UnconfirmedControllers.Num() > 0)
			{
				for (ANetworkController* Controller : FoundComp->UnconfirmedControllers)
				{
					// For all unconfirmed, check if it is the new controller added
					if (NetworkController == Controller)
					{
						// Controller is now valid, so redo creation
						FoundComp->ClientCreated(Controller);
					}
				}
			}
			// Needs to initialise all again
			FoundComp->bHasBeenInitialised = false;

		}

		if (NetworkGameInstance) NetworkGameInstance->HostNumberPlayers = NetworkPlayerControllers.Num();
		// Resets checks to go through creation and initialise again (existing ones will be ignored), ie for lobby
		bControllersCreated = false;
		bPlayerStatesCreated = false;
		bControllersInitialised = false;
		bPlayerStatesInitialised = false;
		bPlayerPawnsCreated = false;
		bPlayerPawnsInitialised = false;

		// Start Checks again
		GetWorldTimerManager().SetTimer(PlayerStateCheckHandle, this, &ANetworkGameMode::SendOutPlayerStateChecks, PlayerStateCheckDelay, false);
		GetWorldTimerManager().SetTimer(ControllerCheckHandle, this, &ANetworkGameMode::SendOutControllerChecks, ControllerCheckDelay, false);

		// Add name to list
		if (NetworkController->PlayerState)
		{
			CurrentNames.AddUnique(NetworkController->PlayerState->GetPlayerName());
		}
	}
}

void ANetworkGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	/*if (bUseSeamlessTravel)
	{
		NetworkGameInstance->HostNumberPlayers--;
	}*/
	NetworkPlayerLeft(Exiting);
}

void ANetworkGameMode::NetworkPlayerLeft(AController* Exiting)
{
	ANetworkController* NetworkController = Cast<ANetworkController>(Exiting);
	if (NetworkController) NetworkPlayerControllers.Remove(NetworkController);
	if (NetworkGameInstance) NetworkGameInstance->HostNumberPlayers = NetworkPlayerControllers.Num();
}

void ANetworkGameMode::SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);

	//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Black, FString("Swap Player Controllers! with GM: ") + GetName());
	NetworkPlayerJoined(NewPC);

}

void ANetworkGameMode::PlayerRejoined(ANetworkController * NetworkController)
{
}

void ANetworkGameMode::SpectatorJoined(ANetworkController * NetworkController)
{

}

TArray<class UNetworkInitialiseComponent*> ANetworkGameMode::GetArrayFromNetworkController(ANetworkController * Controller)
{
	if (ControllersComponentsReady.Contains(Controller))
	{
		return ControllersComponentsReady[Controller];
	}
	return TArray<class UNetworkInitialiseComponent*>();
}

void ANetworkGameMode::NetworkInitialiseComponentDestroyed(UNetworkInitialiseComponent * _NetworkInitialiseComponent)
{
	// Remove from network actors
	NetworkActors.Remove(_NetworkInitialiseComponent);

	// For each controller
	TArray<ANetworkController*> TempControllers;
	ControllersComponentsReady.GetKeys(TempControllers);
	for (ANetworkController* NetworkController : TempControllers)
	{
		// If contains in array, remove
		if (ControllersComponentsReady[NetworkController].Contains(_NetworkInitialiseComponent))
		{
			ControllersComponentsReady[NetworkController].Remove(_NetworkInitialiseComponent);
		}
	}

	// Perform checks to see if player now counts as ready
	for (ANetworkController* NetworkController : NetworkPlayerControllers)
	{
		CheckPlayerFullyReady(NetworkController);
	}

	UCommonFunctions::AddDebugMessage(FString("NetworkInitialiseComponentDestroyed | Component was removed, checking if any players now ready..."), false, bDebugMessages);

}

void ANetworkGameMode::ClearNetworkComponents()
{
	NetworkActors.Empty();
	TArray<ANetworkController*> Controllers;
	ControllersComponentsReady.GenerateKeyArray(Controllers);
	for (ANetworkController* NetworkController : Controllers)
	{
		ControllersComponentsReady[NetworkController].Empty();
	}
}

void ANetworkGameMode::NetworkActorFullyInitialised(UNetworkInitialiseComponent* _NetworkComp, ANetworkController* PlayerControllerSent)
{
	if (!ControllersComponentsReady.Contains(PlayerControllerSent))
	{
		//Player controller not in map
		return;
	}

	// Add completed network comp to array
	ControllersComponentsReady[PlayerControllerSent].AddUnique(_NetworkComp);

	CheckPlayerFullyReady(PlayerControllerSent);
}

void ANetworkGameMode::CheckPlayerFullyReady(ANetworkController* PlayerControllerSent)
{
	if (!ControllersComponentsReady.Contains(PlayerControllerSent))
	{
		//Player controller not in map
		return;
	}

	// Check if all components ready for specific player
	TArray<UNetworkInitialiseComponent*> PlayerComponentsReady = ControllersComponentsReady[PlayerControllerSent];
	if (PlayerComponentsReady.Num() >= NetworkActors.Num())
	{
		PlayerFullyReady(PlayerControllerSent);
	}
}

void ANetworkGameMode::PlayerFullyReady(ANetworkController* PlayerControllerSent)
{
	if (!bControllersCreated)
	{
		// Not all player controllers ready so can't check
		return;
	}

	ANetworkPlayerState* NetworkPlayerState;
	// Check player has player state
	if (!PlayerControllerSent->PlayerState)
	{
		return;
	}
	// Check playe state is network player state
	NetworkPlayerState = Cast<ANetworkPlayerState>(PlayerControllerSent->PlayerState);
	if (!NetworkPlayerState)
	{
		return;
	}
	// Check player state is fully created for all clients
	if (!NetworkPlayerState->AllCreated())
	{
		// If player state is not fully created, can't tell players hes ready
		return;
	}

	// Tell all players this player is ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++)
	{
		NetworkPlayerControllers[i]->MULTI_PlayerReady(NetworkPlayerState);
	}

	CheckAllPlayersFullyReady();
}

void ANetworkGameMode::CheckAllPlayersFullyReady()
{
	// Check all controllers and player states are initialised
	if (!bControllersInitialised)
		return;
	if (!bPlayerStatesInitialised)
		return;

	// Tell all players this player is ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++)
	{
		// Check if all components ready for specific player
		TArray<UNetworkInitialiseComponent*> PlayerComponentsReady = ControllersComponentsReady[NetworkPlayerControllers[i]];
		if (PlayerComponentsReady.Num() < NetworkActors.Num())
		{
			// Not all ready
			return;
		}
	}

	//Can Start

	// (Moved from CheckControllersAndPlayerStatesInitialised())
	AllInitialised();
	CheckPlayerPawnsCreated();

	// Check again in delay time, will get canceled if all are ready
	GetWorldTimerManager().SetTimer(PlayerPawnCheckHandle, this, &ANetworkGameMode::SendOutPlayerPawnChecks, PlayerPawnCheckDelay, false);
}

void ANetworkGameMode::CheckControllersCreated()
{
	// Player Controllers already created
	if (bControllersCreated)
	{
		return;
	}

	// Game Instance not set, attempt to get it
	if (!NetworkGameInstance)
	{
		NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	}
	// Game instance doesn't exist, exit
	if (!NetworkGameInstance)
	{
		UCommonFunctions::AddDebugMessage(FString("CheckControllersCreated | GAME INSTANCE NULL"), true, bDebugMessages);
		return;
	}
	// Player controllers doesn't match host number
	if (NetworkPlayerControllers.Num() < NetworkGameInstance->HostNumberPlayers) // Not all players have joined yet
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not all players Joined"));
		return;
	}

	// Check all controllers ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game States are ready
	{
		if (!NetworkPlayerControllers[i]->AllCreated()) // A player is not ready
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not All Players Ready"));
			return;
		}
	}
	// All controllers ready
	bControllersCreated = true;

	GetWorldTimerManager().ClearTimer(ControllerCheckHandle);
	// Check again in delay time, will get canceled if all are ready
	GetWorldTimerManager().SetTimer(ControllerCheckHandle, this, &ANetworkGameMode::SendOutControllerChecks, ControllerCheckDelay, false);


	// Check all Players ready to see if they are ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers 
	{
		CheckPlayerFullyReady(NetworkPlayerControllers[i]);
	}

	UCommonFunctions::AddDebugMessage(FString("All Controllers created!"), false, bDebugMessages);
	CheckControllersAndPlayerStatesCreated();
}

void ANetworkGameMode::CheckPlayerStatesCreated()
{
	// Player states already created
	if (bPlayerStatesCreated)
	{
		return;
	}

	// Game Instance not set, attempt to get it
	if (!NetworkGameInstance)
	{
		NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	}
	// Game instance doesn't exist, exit
	if (!NetworkGameInstance)
	{
		UCommonFunctions::AddDebugMessage(FString("CheckPlayerStatesCreated | GAME INSTANCE NULL"), true, bDebugMessages);
		return;
	}

	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	if (PlayerStates.Num() < NetworkGameInstance->HostNumberPlayers) // Not all player states have been spawned
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not all players Joined"));
		return;
	}

	// Check all Player States ready
	for (int i = 0; i < PlayerStates.Num(); i++) // Checking all Controllers and Game States are ready
	{
		ANetworkPlayerState* NetworkPlayerState = Cast<ANetworkPlayerState>(PlayerStates[i]);
		if (!NetworkPlayerState) // Player State invalid, Exit
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Player State Invalid"));
			return;
		}
		if (!NetworkPlayerState->AllCreated()) // Player State not ready
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not All player States Ready"));
			return;
		}
	}
	// All Player States ready
	bPlayerStatesCreated = true;

	GetWorldTimerManager().ClearTimer(PlayerStateCheckHandle);

	// Check all Players ready to see if they are ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers 
	{		
		CheckPlayerFullyReady(NetworkPlayerControllers[i]);
	}

	UCommonFunctions::AddDebugMessage(FString("All Player States created!"), false, bDebugMessages);
	CheckControllersAndPlayerStatesCreated();
}

void ANetworkGameMode::SendOutPlayerStateChecks()
{
	// Player stats all created, no need to send out check
	if (bPlayerStatesCreated)
	{
		// Check if not initialised
		if (bPlayerStatesInitialised)
			return;
	}
	UCommonFunctions::AddDebugMessage(FString("Send Out Player State " + FString(bPlayerStatesInitialised ? "Init" : "Creation") + " Checks!"), false, bDebugMessages);

	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;

	// Check all Player States ready
	for (int i = 0; i < PlayerStates.Num(); i++) // Checking all Controllers and Game States are ready
	{
		ANetworkPlayerState* NetworkPlayerState = Cast<ANetworkPlayerState>(PlayerStates[i]);
		if (!NetworkPlayerState) // Player State invalid, continue
		{
			continue;
		}

		if (bPlayerStatesCreated) // Controllers have been created, so initialised instead
		{
			if (!NetworkPlayerState->AllInitialised()) // Player State not ready
			{
				// Send check to clients not created
				NetworkPlayerState->S_InitialisedCheck();
				//NetworkPlayerState->MULTI_Initialize();
			}
		}
		else
		{
			if (!NetworkPlayerState->AllCreated()) // Player State not ready
			{
				// Send check to clients not created
				NetworkPlayerState->S_CreatedCheck();
			}
		}

	}
	// Check again in delay time, will get canceled if all are ready
	GetWorldTimerManager().SetTimer(PlayerStateCheckHandle, this, &ANetworkGameMode::SendOutPlayerStateChecks, PlayerStateCheckDelay, false);
}

void ANetworkGameMode::SendOutControllerChecks()
{
	// Player stats all created, no need to send out check
	if (bControllersCreated)
	{
		// Check if not initialised
		if (bControllersInitialised)
			return;
	}

	UCommonFunctions::AddDebugMessage(FString("Send Out Controllers  " + FString(bControllersInitialised ? "Init" : "Creation") + "Checks!"), false, bDebugMessages);

	// Check all controllers ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game States are ready
	{
		if (bControllersCreated) // Controllers have been created, so initialised instead
		{
			if (!NetworkPlayerControllers[i]->AllInitialised()) // A player is not initialised
			{
				// Send to initialise players that aren't initialised
				NetworkPlayerControllers[i]->MULTI_Initialize();
			}
		}
		else
		{
			if (!NetworkPlayerControllers[i]->AllCreated()) // A player is not ready
			{
				// Send check to clients not created
				NetworkPlayerControllers[i]->MULTI_CreatedCheck();
			}
		}
	}

	// Check again in delay time, will get canceled if all are ready
	GetWorldTimerManager().SetTimer(ControllerCheckHandle, this, &ANetworkGameMode::SendOutControllerChecks, ControllerCheckDelay, false);
}

void ANetworkGameMode::CheckControllersAndPlayerStatesCreated()
{
	// Not all controllers and player states created
	if (!bControllersCreated || !bPlayerStatesCreated)
	{
		return;
	}

	// If game in progress
	if (GameStarted)
	{
		bool JoinInProgress = false;
		if (NetworkGameInstance)
		{
			JoinInProgress = NetworkGameInstance->bJoinInProgress;
		}

		for (ANetworkController* NetworkController : NetworkPlayerControllers)
		{
			if (!NetworkController) // A player is not valid
			{
				continue;
			}
			// check has not been initialised before, meaning is new player
			if (!NetworkController->bHasBeenInitialised)
			{
				if (NetworkController->PlayerState)
				{
					// check if original player or join in progress
					if (CurrentNames.Contains(NetworkController->PlayerState->GetPlayerName()) || JoinInProgress)
					{
						// Player Rejoined
						PlayerRejoined(NetworkController);
					}
					else // Isin't original player, go spectate
					{
						// Spectate
						/// WONT BE CALLED, SPECTATOR NOT ADDED TO PLAYER CONTROLLERS ARRAY
						SpectatorJoined(NetworkController);
					}
				}
			}
		}
	}


	// Initialise all controllers
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game States are ready
	{
		if (!NetworkPlayerControllers[i]) // A player is not valid
		{
			UCommonFunctions::AddDebugMessage(FString("GM CheckControllersAndPlayerStatesCreated | PLAYER CONTROLLER IS NULL"), true, bDebugMessages);
			continue;
		}
		NetworkPlayerControllers[i]->MULTI_Initialize();
	}

	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	// Initialise all player states
	for (int i = 0; i < PlayerStates.Num(); i++) // Checking all Controllers and Game States are ready
	{
		ANetworkPlayerState* NetworkPlayerState = Cast<ANetworkPlayerState>(PlayerStates[i]);
		if (!NetworkPlayerState) // Player State invalid, Exit
		{
			UCommonFunctions::AddDebugMessage(FString("GM CheckControllersAndPlayerStatesCreated | PLAYER STATE IS NULL/NOT NETWORK"), true, bDebugMessages);
			continue;
		}
		NetworkPlayerState->MULTI_Initialize();
	}

	// Start delay time for player states initialise
	GetWorldTimerManager().SetTimer(PlayerStateCheckHandle, this, &ANetworkGameMode::SendOutPlayerStateChecks, PlayerStateCheckDelay, false);
	// Start delay time for player controllers initialise
	GetWorldTimerManager().SetTimer(ControllerCheckHandle, this, &ANetworkGameMode::SendOutControllerChecks, ControllerCheckDelay, false);

}

void ANetworkGameMode::CheckControllersInitialised()
{
	// Player Controllers already Initialised
	if (bControllersInitialised)
	{
		return;
	}

	// Check all controllers ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game States are ready
	{
		if (!NetworkPlayerControllers[i]->AllInitialised()) // A player is not ready
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not All Players Ready"));
			return;
		}
	}
	// All controllers ready
	bControllersInitialised = true;
	GetWorldTimerManager().ClearTimer(ControllerCheckHandle);
	UCommonFunctions::AddDebugMessage(FString("All Controllers initialised!"), false, bDebugMessages);
	CheckControllersAndPlayerStatesInitialised();
}

void ANetworkGameMode::CheckPlayerStatesInitialised()
{
	// Player states already initialised
	if (bPlayerStatesInitialised)
	{
		return;
	}

	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	// Check all Player States ready
	for (int i = 0; i < PlayerStates.Num(); i++) // Checking all Controllers and Game States are ready
	{
		ANetworkPlayerState* NetworkPlayerState = Cast<ANetworkPlayerState>(PlayerStates[i]);
		if (!NetworkPlayerState) // Player State invalid, Exit
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Player State Invalid"));
			return;
		}
		if (!NetworkPlayerState->AllInitialised()) // Player State not ready
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not All player States Ready"));
			return;
		}
	}
	// All Player States ready
	bPlayerStatesInitialised = true;
	GetWorldTimerManager().ClearTimer(PlayerStateCheckHandle);

	UCommonFunctions::AddDebugMessage(FString("All Player States initialised!"), false, bDebugMessages);
	CheckControllersAndPlayerStatesInitialised();
}

void ANetworkGameMode::CheckControllersAndPlayerStatesInitialised()
{
	// Not all controllers and player states created
	if (!bControllersInitialised || !bPlayerStatesInitialised)
	{
		return;
	}

	UCommonFunctions::AddDebugMessage(FString("All Player States and Controllers initialised! Starting Pawns..."), false, bDebugMessages);

	CheckAllPlayersFullyReady();

}

void ANetworkGameMode::IndividualPlayerPawnsCheck()
{
	//TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;

	bool AllCreated = true;
	bool AllInitialised = true;

	// Check all Player Pawns ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game Pawns are ready
	{
		if (!NetworkPlayerControllers[i])
			continue;
		ANetworkCharacter* NetworkPlayerPawn = Cast<ANetworkCharacter>(NetworkPlayerControllers[i]->GetPawn());
		if (!NetworkPlayerPawn) // Player Pawn invalid, Exit
		{
			UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | Player Pawn Invalid"), true, bDebugMessages);
			continue;
		}
		if (NetworkPlayerPawn->NetworkInitialiseComp->AllCreated()) // Player Pawn created
		{
			if (!NetworkPlayerPawn->NetworkInitialiseComp->AllInitialised() && !NetworkPlayerPawn->NetworkInitialiseComp->bHasBeenInitialised) // Player Pawn not ready
			{
				// Initialise since pawn is not initialised
				NetworkPlayerPawn->NetworkInitialiseComp->MULTI_Initialize();
				// Not all initialised
				AllInitialised = false;
			}
		}
		else
		{

			// Not all created
			AllCreated = false;
			// Not all initialised
			AllInitialised = false;
		}
	}

	// If not fully initialised or created
	if (!AllCreated || !AllInitialised)
	{
		// Start delay time for player Pawns initialise
		GetWorldTimerManager().SetTimer(PlayerPawnCheckHandle, this, &ANetworkGameMode::SendOutPlayerPawnChecks, PlayerPawnCheckDelay, false);

		// Set all pawns created check
		if (!AllCreated)
			bPlayerPawnsCreated = false;

		// Set all pawns initialised check
		if (!AllInitialised)
			bPlayerPawnsInitialised = false;
	}

	if (AllCreated)
	{
		// All created so reset value
		iCurrentPawnCheckCount = 0;
	}

	//if (AllCreated)
	//{
	//	// Pawns weren't fully created yet
	//	if (!bPlayerPawnsCreated)
	//	{
	//		// Safety check for player pawns not initialised
	//		if (!bPlayerPawnsInitialised)
	//		{
	//			// Start delay time for player Pawns initialise
	//			GetWorldTimerManager().SetTimer(PlayerPawnCheckHandle, this, &ANetworkGameMode::SendOutPlayerPawnChecks, PlayerPawnCheckDelay, false);
	//		}
	//	}
	//	// All Player Pawns ready
	//	bPlayerPawnsCreated = true;
	//	GetWorldTimerManager().ClearTimer(PlayerPawnCheckHandle);
	//}

	// All initialised, stop check
	if (AllInitialised)
	{
		// All Player Pawns ready
		bPlayerPawnsInitialised = true;
		GetWorldTimerManager().ClearTimer(PlayerPawnCheckHandle);

		// Might be risky
		bPlayerPawnsCreated = true;
	}

}

void ANetworkGameMode::ResetControllersStatesSetup()
{
	bControllersCreated = false;
	bPlayerStatesCreated = false;
	bControllersInitialised = false;
	bPlayerStatesInitialised = false;
}

void ANetworkGameMode::ResetPawnSetup()
{
	bPlayerPawnsCreated = false;
	bPlayerPawnsInitialised = false;
}


void ANetworkGameMode::CheckPlayerPawnsCreated()
{
	// Game in progression, do individual pawn check
	if (GameStarted || ProgressiveJoining)
	{
		IndividualPlayerPawnsCheck();
		return;
	}

	// Player Pawns already created
	if (bPlayerPawnsCreated && !GameStarted)
	{
		iCurrentPawnCheckCount = 0;
		return;
	}

	// Not all controllers and player states created
	if (!bControllersInitialised || !bPlayerStatesInitialised)
	{
		return;
	}

	// Game Instance not set, attempt to get it
	if (!NetworkGameInstance)
	{
		NetworkGameInstance = Cast<UNWGameInstance>(GetGameInstance());
	}
	// Game instance doesn't exist, exit
	if (!NetworkGameInstance)
	{
		UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | GAME INSTANCE NULL"), true, bDebugMessages);
		return;
	}

	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	if (PlayerStates.Num() < NetworkGameInstance->HostNumberPlayers) // Not all player Pawns have been spawned
	{
		UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | PlayerStates Num smaller than GI players num"), true, bDebugMessages);
		return;
	}

	// Check all Player Pawns ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game Pawns are ready
	{
		if (!NetworkPlayerControllers[i])
			continue;
		ANetworkCharacter* NetworkPlayerPawn = Cast<ANetworkCharacter>(NetworkPlayerControllers[i]->GetPawn());
 		if (!NetworkPlayerPawn) // Player Pawn invalid, Exit
		{
			UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | Player Pawn Invalid"), true, bDebugMessages);
			return;
		}
		if (!NetworkPlayerPawn->NetworkInitialiseComp) // Player Pawn not ready
		{
			UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | Player Pawn Network Initialise Component Invalid"), true, bDebugMessages);
			return;
		}
		if (!NetworkPlayerPawn->NetworkInitialiseComp->AllCreated()) // Player Pawn not ready
		{
			UCommonFunctions::AddDebugMessage(FString("CheckPlayerPawnsCreated | Not All player Pawns Ready"), true, bDebugMessages);
			return;
		}
	}
	// All Player Pawns ready
	bPlayerPawnsCreated = true;
	GetWorldTimerManager().ClearTimer(PlayerPawnCheckHandle);

	UCommonFunctions::AddDebugMessage(FString("All Player Pawns created!"), false, bDebugMessages);

	InitialisePlayerPawns();
}

void ANetworkGameMode::InitialisePlayerPawns()
{
	iCurrentPawnCheckCount = 0;
	//TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	// Initialise all player Pawns
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game Pawns are ready
	{
		if (!NetworkPlayerControllers[i])
			continue;
		ANetworkCharacter* NetworkPlayerPawn = Cast<ANetworkCharacter>(NetworkPlayerControllers[i]->GetPawn());
		if (!NetworkPlayerPawn) // Player Pawn invalid, Exit
		{
			UCommonFunctions::AddDebugMessage(FString("GM InitialisePlayerPawns | PLAYER Pawn IS NULL/NOT NETWORK"), true, bDebugMessages);
			continue;
		}
		NetworkPlayerPawn->NetworkInitialiseComp->MULTI_Initialize();
	}

	// Start delay time for player Pawns initialise
	GetWorldTimerManager().SetTimer(PlayerPawnCheckHandle, this, &ANetworkGameMode::SendOutPlayerPawnChecks, PlayerPawnCheckDelay, false);
}

void ANetworkGameMode::CheckPlayerPawnsInitialised()
{
	// Game in progression, do individual pawn check
	if (GameStarted || ProgressiveJoining)
	{
		IndividualPlayerPawnsCheck();
		return;
	}

	// Player Pawns already initialised
	if (bPlayerPawnsInitialised && !GameStarted)
	{
		return;
	}

	
	//TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	// Check all Player Pawns ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game Pawns are ready
	{
		if (!NetworkPlayerControllers[i])
			continue;
		ANetworkCharacter* NetworkPlayerPawn = Cast<ANetworkCharacter>(NetworkPlayerControllers[i]->GetPawn());
		if (!NetworkPlayerPawn) // Player Pawn invalid, Exit
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Player Pawn Invalid"));
			return;
		}
		if (!NetworkPlayerPawn->NetworkInitialiseComp->AllInitialised()) // Player Pawn not ready
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Not All player Pawns Ready"));
			return;
		}
	}
	// All Player Pawns ready
	bPlayerPawnsInitialised = true;
	GetWorldTimerManager().ClearTimer(PlayerPawnCheckHandle);

	// Might be risky
	bPlayerPawnsCreated = true;

	UCommonFunctions::AddDebugMessage(FString("All Player Pawns initialised!"), false, bDebugMessages);

	// ALL INIT
	if (!GameStarted)
		AllPawnsInitialised();
}
void ANetworkGameMode::SendOutPlayerPawnChecks()
{
	// Player stats all created, no need to send out check
	if (bPlayerPawnsCreated)
	{
		// Check if not initialised
		if (bPlayerPawnsInitialised)
		{
			return;
		}
	}
	UCommonFunctions::AddDebugMessage(FString("Send Out Player Pawn Checks!"), false, bDebugMessages);

	//TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;

	// Check all Player Pawns ready
	for (int i = 0; i < NetworkPlayerControllers.Num(); i++) // Checking all Controllers and Game Pawns are ready
	{
		if (!NetworkPlayerControllers[i])
			continue;
		ANetworkCharacter* NetworkPlayerPawn = Cast<ANetworkCharacter>(NetworkPlayerControllers[i]->GetPawn());
		if (!NetworkPlayerPawn) // Player Pawn invalid, continue
		{
			continue;
		}

		if (bPlayerPawnsCreated || GameStarted) // Controllers have been created, so initialised instead
		{
			if (!NetworkPlayerPawn->NetworkInitialiseComp->AllInitialised()) // Player Pawn not ready
			{
				// Send check to clients not created
				NetworkPlayerPawn->NetworkInitialiseComp->S_InitialisedCheck();
				//NetworkPlayerPawn->MULTI_Initialize();
			}
		}
		if (!bPlayerPawnsCreated || GameStarted)
		{
			if (!NetworkPlayerPawn->NetworkInitialiseComp->AllCreated()) // Player Pawn not ready
			{
				if (iCurrentPawnCheckCount > iMaxPawnCheckCount)
				{
					FVector SpawnPos = NetworkPlayerPawn->GetActorLocation();
					if (NetworkPlayerPawn)
						NetworkPlayerPawn->Destroy();
					SpawnPlayerPawn(NetworkPlayerControllers[i], SpawnPos);
				}
				else
				{
					// Send check to clients not created
					NetworkPlayerPawn->NetworkInitialiseComp->S_CreatedCheck();
				}

			}
		}

	}

	iCurrentPawnCheckCount++;

	// Check again in delay time, will get canceled if all are ready
	GetWorldTimerManager().SetTimer(PlayerPawnCheckHandle, this, &ANetworkGameMode::SendOutPlayerPawnChecks, PlayerPawnCheckDelay, false);
}

