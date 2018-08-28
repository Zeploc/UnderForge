// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgePlayer.h"
//#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AForgePlayer::AForgePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AForgePlayer::Interact);
}
void AForgePlayer::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
}

void AForgePlayer::MoveUp(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void AForgePlayer::Interact()
{
	FHitResult hit;
	FVector EndLocation = GetActorLocation() + GetActorRotation().Vector() * InteractRange;
	FCollisionQueryParams Traceparams(TEXT("Interact Trace"), false, this);

	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), EndLocation, ECC_GameTraceChannel1, Traceparams);
	//AInteractActor* InteractActor = Cast<AInteractActor>(hit.Actor);
	//if (InteractActor)
	//{
	//	InteractActor->Interact();
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Interact with interact actor"));
	//}
	//else if (hit.bBlockingHit)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Interact Hit object " + hit.Actor.Get()->GetFName().ToString()));
	//}

}