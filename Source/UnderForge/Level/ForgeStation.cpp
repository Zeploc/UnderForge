// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AForgeStation::AForgeStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh"));
	RootComponent = StationMesh;
	ObjectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Part Position"));
	ObjectPosition->SetupAttachment(StationMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(StationMesh);
}

// Called when the game starts or when spawned
void AForgeStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AForgeStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

