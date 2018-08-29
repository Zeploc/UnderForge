// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemiesZombo.h"


// Sets default values
AEnemiesZombo::AEnemiesZombo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemiesZombo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemiesZombo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemiesZombo::Seeker(FVector location)
{
	
}

