// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"
class AForgePlayer;
class AEnemiesZombo;
UCLASS()
class UNDERFORGE_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<AForgePlayer*> PlayerArray;
	TArray<AEnemiesZombo*> EnemyArray;

	UPROPERTY(EditAnywhere,BlueprintReadWrite) 
		TArray<FVector> SpawnLocations;

	UPROPERTY(EditAnywhere, Category = Enemy)
		TSubclassOf<class AEnemiesZombo> EnemyZomboClass;

	FTimerHandle timer;
};