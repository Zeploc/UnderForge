// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgePart.h"
#include "Components/StaticMeshComponent.h"
#include "Level/ForgeStation.h"
#include "Engine/World.h"
#include "Mechanics/ForgeLevel.h"

// Sets default values
AForgePart::AForgePart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mat Mesh"));
	RootComponent = PartMesh;
	PartMesh->SetSimulatePhysics(true);
	PartMesh->SetCollisionProfileName("PhysicsActor");
	PartMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	bReplicates = true;
	bReplayRewindable = true;

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
}

// Called every frame
void AForgePart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AForgePart::DropOnStation()
{
	if (CurrentTouchingStation)
	{
		CurrentTouchingStation->ProcessPartItem(this);
	}
}

