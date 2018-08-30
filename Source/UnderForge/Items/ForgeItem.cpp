// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeItem.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AForgeItem::AForgeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionProfileName("PhysicsActor");
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AForgeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AForgeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


