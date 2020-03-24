// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeMat.h"
#include "Level/ForgeStation.h"
#include "Player/ForgePlayer.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AForgeMat::AForgeMat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AttachOffset.SetScale3D(FVector(0.5f));

	bReplicates = true;
	bReplicateMovement = true;
	PickUpType = EPickUpType::PT_MAT;
}

// Called when the game starts or when spawned
void AForgeMat::BeginPlay()
{
	Super::BeginPlay();
	
	// Sync up client to use servers version
	if (GetOwner() && !HasAuthority())
	{
		if (AForgePlayer* OwnerForgePlayer = Cast<AForgePlayer>(GetOwner()))
		{
			if (OwnerForgePlayer->HoldItem)
			{
				if (AForgeMat* HeldMat = Cast<AForgeMat>(OwnerForgePlayer->HoldItem))
				{
					if (HeldMat->ResourceType == ResourceType)
					{
						HeldMat->Destroy();
						PickUp(OwnerForgePlayer);
						OwnerForgePlayer->HoldItem = this;
						
					}
				}
			}
		}
	}
}

// Called every frame
void AForgeMat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

