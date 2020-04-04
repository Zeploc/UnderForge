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
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
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
	PlayerInputComponent->BindAxis("SpinningXAxis", this, &AForgePlayer::SetXValue);
	PlayerInputComponent->BindAxis("SpinningYAxis", this, &AForgePlayer::SetYValue);
	PlayerInputComponent->BindAction("SecondaryInteract", IE_Pressed, this, &AForgePlayer::SecondaryInteract);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AForgePlayer::Interact);
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

bool AForgePlayer::Interact()
{
	FHitResult hit;
	FVector EndLocation = GetActorLocation() + GetActorRotation().Vector() * InteractRange;
	FCollisionQueryParams Traceparams(TEXT("Interact Trace"), false, this);
	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), EndLocation, ECC_Station, Traceparams);
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	if (AForgeStation* Station = Cast<AForgeStation>(hit.Actor))
	{
		return Station->TryInteract(this);
	}
	return false;
}

void AForgePlayer::ClearHoldItem()
{
	if (HasAuthority())
	{
		MULTI_ClearHoldItem();
	}
	else
	{
		SERVER_ClearHoldItem();
		MULTI_ClearHoldItem_Implementation();
	}
}

void AForgePlayer::SERVER_ClearHoldItem_Implementation()
{
	MULTI_ClearHoldItem();
}
bool AForgePlayer::SERVER_ClearHoldItem_Validate()
{
	return true;
}

void AForgePlayer::MULTI_ClearHoldItem_Implementation()
{
	if (HoldItem)
	{
		if (HoldItem->IsAttachedTo(this))
		{
			HoldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
	HoldItem = nullptr;
}

void AForgePlayer::SetHoldItem(APickUpItem * PickUp)
{
	if (HasAuthority())
	{
		MULTI_SetHoldItem(PickUp);
	}
	else
	{
		SERVER_SetHoldItem(PickUp);
		MULTI_SetHoldItem_Implementation(PickUp);
	}
}

void AForgePlayer::SERVER_SetHoldItem_Implementation(APickUpItem * PickUp)
{
	MULTI_SetHoldItem(PickUp);
}
bool AForgePlayer::SERVER_SetHoldItem_Validate(APickUpItem * PickUp)
{
	return true;
}

void AForgePlayer::MULTI_SetHoldItem_Implementation(APickUpItem * PickUp)
{
	HoldItem = PickUp;
}

void AForgePlayer::SERVER_InteractWith_Implementation(AForgeStation * Station)
{
	Station->TryInteract(this);
}
bool AForgePlayer::SERVER_InteractWith_Validate(AForgeStation * Station)
{
	return true;
}

void AForgePlayer::CLIENT_PickUp_Implementation(APickUpItem * PickUp)
{
	if (!PickUp)
		return;

	PickUp->PickUp(this);
	HoldItem = PickUp;
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
					if (mat->CurrentStation && mat->ResourceType == EResource::R_WOOD)
					{
						if (ACarpentaryStation* station = Cast<ACarpentaryStation>(mat->CurrentStation))
						{
							station->MorphStates();
						}
					}
				}
			}
		}
	}*/
}

void AForgePlayer::StartAttack_Implementation()
{
}

void AForgePlayer::LockPosition(bool IsLocked)
{
	if (IsLocked)
		DisableInput(Cast<APlayerController>(GetController()));
	else
		EnableInput(Cast<APlayerController>(GetController()));
}



//TEMP

#include "Engine.h"


bool AForgePlayer::HoldingWeapon()
{
	if (!HoldItem)
		return false;
	return (HoldItem->PickUpType == EPickUpType::PT_WEAPON);
}

bool AForgePlayer::HoldingHammer()
{
	if (!HoldItem)
		return false;
	return (HoldItem->ToolType == EToolType::TT_HAMMER);
}


void AForgePlayer::SetXValue(float x)
{
	CurrentX = x;
}

void AForgePlayer::SetYValue(float y)
{
	CurrentY = y;
}