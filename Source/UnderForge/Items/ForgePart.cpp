// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgePart.h"
#include "Components/StaticMeshComponent.h"
#include "Level/ForgeStation.h"
#include "Player/ForgePlayer.h"
#include "Engine/World.h"
#include "Mechanics/ForgeLevel.h"

// Sets default values
AForgePart::AForgePart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;
	bReplicateMovement = true;
	PickUpType = EPickUpType::PT_PART;

	AttachOffset.SetScale3D(FVector(0.5f));
}

// Called when the game starts or when spawned
void AForgePart::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		if (GetWorld()->GetAuthGameMode())
		{
			Cast<AForgeLevel>(GetWorld()->GetAuthGameMode())->TotalPartsCreated++;
		}
	}

	// Sync up client to use servers version
	/*if (GetOwner() && !HasAuthority())
	{
		if (AForgePlayer* OwnerForgePlayer = Cast<AForgePlayer>(GetOwner()))
		{
			if (OwnerForgePlayer->GetHoldItem())
			{
				if (AForgePart* HeldMat = Cast<AForgePart>(OwnerForgePlayer->GetHoldItem()))
				{
					if (HeldMat->SwordPart == SwordPart)
					{
						HeldMat->Destroy();
						PickUp(OwnerForgePlayer);
						OwnerForgePlayer->SetHoldItem(this);
					}
				}
			}
		}
	}*/
}

// Called every frame
void AForgePart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


