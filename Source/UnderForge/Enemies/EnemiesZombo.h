// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemiesZombo.generated.h"

class UStaticMeshComponent;
UCLASS()
class UNDERFORGE_API AEnemiesZombo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesZombo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* selfMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Seeker(FVector location);
	int iHealth;
	UFUNCTION(BlueprintCallable)
		void TakeDamage(int iDamage, FVector HitFrom);


};
