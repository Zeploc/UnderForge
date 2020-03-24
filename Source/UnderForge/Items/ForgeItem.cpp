// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeItem.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Mechanics/ForgeLevel.h"

// Sets default values
AForgeItem::AForgeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = true;
	PickUpType = EPickUpType::PT_WEAPON;


	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Box"));
	InteractBox->SetupAttachment(ItemMesh);

}

// Called when the game starts or when spawned
void AForgeItem::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (GetWorld()->GetAuthGameMode())
		{
			Cast<AForgeLevel>(GetWorld()->GetAuthGameMode())->TotalPartsCreated++;
		}
	}
}

// Called every frame
void AForgeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


