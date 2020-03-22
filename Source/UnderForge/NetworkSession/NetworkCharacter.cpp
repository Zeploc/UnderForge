// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkCharacter.h"

#include "NetworkSession/NetworkGameMode.h"
#include "NetworkSession/NetworkController.h"
#include "NetworkSession/NWGameInstance.h"
#include "NetworkSession/NetworkInitialiseComponent.h"

#include "Engine/World.h"

// Sets default values
ANetworkCharacter::ANetworkCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NetworkInitialiseComp = CreateDefaultSubobject<UNetworkInitialiseComponent>(TEXT("Network Initialise Component"));
}

// Called when the game starts or when spawned
void ANetworkCharacter::BeginPlay()
{
	Super::BeginPlay();

	//GameModeRef = Cast<ANetworkGameMode>(GetWorld()->GetAuthGameMode());
	//GameInstanceRef = Cast<UNWGameInstance>(GetGameInstance());

	//if (GameModeRef)
	//{
	//	S_UpdateControllersMap();
	//}
	//PlayerPawnCreated();
}

// Called every frame
void ANetworkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANetworkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANetworkCharacter::Initialise_Implementation()
{
	OnInitialize();


}

void ANetworkCharacter::FullyInitialised_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "RUNE INITIALISE COMPLETE");
	if (NetworkInitialiseComp)
		NetworkInitialiseComp->GameModeRef->CheckPlayerPawnsInitialised();
}

bool ANetworkCharacter::FinishedCreation_Implementation()
{
	if (NetworkInitialiseComp)
		NetworkInitialiseComp->GameModeRef->CheckPlayerPawnsCreated();
	// Shouldn't indivdually continue
	return false;
}

void ANetworkCharacter::OnInitialize()
{
}
