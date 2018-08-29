// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeMat.h"
#include "Level/ForgeStation.h"
#include "Level/Smeltery.h"
#include "Level/CarpentaryStation.h"
#include "Utlities.h"
// Sets default values
AForgeMat::AForgeMat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		CurrentTouchingStation->ProcessItem(this);
	}
}

// Called every frame
void AForgeMat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

