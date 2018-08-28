// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraSystem.h"


// Sets default values
ACameraSystem::ACameraSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

