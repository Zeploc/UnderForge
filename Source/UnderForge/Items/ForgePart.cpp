// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgePart.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AForgePart::AForgePart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mat Mesh"));
	RootComponent = PartMesh;
	PartMesh->SetSimulatePhysics(true);
	PartMesh->SetCollisionProfileName("PhysicsActor");
	PartMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AForgePart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AForgePart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

