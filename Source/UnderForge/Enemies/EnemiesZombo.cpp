// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemiesZombo.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemiesZombo::AEnemiesZombo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	iHealth = 100;
}

// Called when the game starts or when spawned
void AEnemiesZombo::BeginPlay()
{
	Super::BeginPlay();
	selfMesh = FindComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void AEnemiesZombo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (iHealth <= 0)
	{
		Destroy();
	}
}

void AEnemiesZombo::Seeker(FVector location)
{
	
}

void AEnemiesZombo::TakeDamage(int iDamage, FVector HitFrom)
{
	iHealth -= iDamage;
	FVector force = GetActorLocation() - HitFrom;
	force.Normalize();
	force *= (25.0f * iDamage);
	selfMesh->AddImpulse(force,NAME_None,true);
}

