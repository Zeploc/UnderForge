// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeMat.h"
#include "Level/ForgeStation.h"

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
	
}

// Called every frame
void AForgeMat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

