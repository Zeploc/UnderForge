// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForgeStation.generated.h"

class AForgeMat;

UCLASS()
class UNDERFORGE_API AForgeStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForgeStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* ObjectPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionBox;
};
