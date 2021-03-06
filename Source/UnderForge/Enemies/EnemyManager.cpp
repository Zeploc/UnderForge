// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Player/ForgePlayer.h"
#include "EnemiesZombo.h"
#include "Engine.h"
// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> TempArray;
	UGameplayStatics::GetAllActorsOfClass(this, AForgePlayer::StaticClass(), TempArray);
	for(auto it: TempArray)
	{
		if (Cast<AForgePlayer>(it))
		{
			PlayerArray.Push(Cast<AForgePlayer>(it));
		}
	}

	//SpawnLocations.Push(FVector(740.0f, -215.0f, 77.0f));
	//SpawnLocations.Push(FVector(590.0f, -290.0f, 77.0f));
	//SpawnLocations.Push(FVector(590.0f, 85.0f, 77.0f));
	SpawnLocations.Push(FVector(665.0f, -65.0, 77.0f));
	

	GetWorld()->GetTimerManager().SetTimer(timer, this, &AEnemyManager::SpawnEnemy, 1.0f, true);
}

void AEnemyManager::SpawnEnemy()
{
	if (EnemyZomboClass)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			int random = rand() % SpawnLocations.Num();
			FRotator rotator;
			FVector spawnLoc = SpawnLocations[random];
			EnemyArray.Push(Cast<AEnemiesZombo>(world->SpawnActor<AEnemiesZombo>(EnemyZomboClass, spawnLoc, rotator, spawnParams)));
		}
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

