// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgePlayer.h"
//#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/ForgeMat.h"
#include "Items/ForgePart.h"
#include "Utlities.h"
#include "Level/CarpentaryStation.h"
#include "Level/Smeltery.h"
#include "Level/ForgeAnvil.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AForgePlayer::AForgePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AForgePlayer::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AForgePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AForgePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &AForgePlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AForgePlayer::MoveUp);
	PlayerInputComponent->BindAction("SecondaryInteract", IE_Pressed, this, &AForgePlayer::SecondaryInteract);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AForgePlayer::Interact);
}
void AForgePlayer::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void AForgePlayer::MoveUp(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AForgePlayer::Interact()
{
	FHitResult hit;
	FVector EndLocation = GetActorLocation() + GetActorRotation().Vector() * InteractRange;
	FCollisionQueryParams Traceparams(TEXT("Interact Trace"), false, this);
	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), EndLocation, ECC_Station, Traceparams);
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	if (ASmeltery* smelty = Cast<ASmeltery>(hit.Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Minigame Start"));
		if (smelty->bSmeltingMinigamePlaying)
		{
			UE_LOG(LogTemp, Warning, TEXT("Minigame Finish"));
			smelty->MiniGameComplete();
		}
	}
	else if (AForgeAnvil* anvil = Cast<AForgeAnvil>(hit.Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Minigame Start"));
		if (anvil->bHammerMinigamePlaying)
		{
			UE_LOG(LogTemp, Warning, TEXT("Minigame Finish"));
			anvil->HammeringCycle();
		}
	}
}

void AForgePlayer::SecondaryInteract()
{
	/*if (FindComponentByClass<UPhysicsHandleComponent>()->GetGrabbedComponent())
	{
		if (Cast<UStaticMeshComponent>(FindComponentByClass<UPhysicsHandleComponent>()->GetGrabbedComponent()))
		{
			if (AActor* item = Cast<UStaticMeshComponent>(FindComponentByClass<UPhysicsHandleComponent>()->GetGrabbedComponent())->GetOwner())
			{
				if (AForgeMat* mat = Cast<AForgeMat>(item))
				{
					if (mat->CurrentTouchingStation && mat->ResourceType == EResource::R_WOOD)
					{
						if (ACarpentaryStation* station = Cast<ACarpentaryStation>(mat->CurrentTouchingStation))
						{
							station->MorphStates();
						}
					}
				}
			}
		}
	}*/
}
