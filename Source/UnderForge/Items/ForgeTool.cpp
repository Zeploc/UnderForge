// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeTool.h"

#include "Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
AForgeTool::AForgeTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
	RootComponent = ToolMesh;
	ToolMesh->SetSimulatePhysics(true);
	ToolMesh->SetCollisionProfileName("PhysicsActor");
	ToolMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
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

void AForgeTool::PickUp(AForgePlayer * NewPlayer)
{
	IsHeld = true;
	CurrentPlayer = NewPlayer;
	ToolMesh->SetSimulatePhysics(false);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Tool Picked Up");
}

void AForgeTool::Drop()
{
	IsHeld = false;
	CurrentPlayer = nullptr;
	ToolMesh->SetSimulatePhysics(true);
}

