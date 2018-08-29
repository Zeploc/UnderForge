// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "CombineBench.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API ACombineBench : public AForgeStation
{
	GENERATED_BODY()

public:

	ACombineBench();

	void VirtualOverrideFunc(AActor* OverlappActor);
	void ChangeMesh();
	void ThrowAway(AActor* Actor);
	
	TArray<class AForgePart*> CurrentParts;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentWeaponMesh;
};
