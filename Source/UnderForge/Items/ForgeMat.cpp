// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeMat.h"
#include "Level/ForgeStation.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AForgeMat::AForgeMat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mat Mesh"));
	RootComponent = MatMesh;
	MatMesh->SetSimulatePhysics(true);
	MatMesh->SetCollisionProfileName("PhysicsActor");
	MatMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AForgeMat::BeginPlay()
{
	Super::BeginPlay();
	
}

void AForgeMat::DropOnStation()
{
	if (CurrentTouchingStation)
	{
		CurrentTouchingStation->ProcessMatItem(this);
	}
}

// Called every frame
void AForgeMat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

