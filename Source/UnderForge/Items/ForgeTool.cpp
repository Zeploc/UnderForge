// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeTool.h"

#include "Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
AForgeTool::AForgeTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	bReplicates = true;
	bReplicateMovement = true;
	PickUpType = EPickUpType::PT_TOOL;
	iAttackDamage = 20;
	
}

// Called when the game starts or when spawned
void AForgeTool::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AForgeTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


