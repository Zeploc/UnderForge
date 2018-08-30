// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "CarpentaryStation.generated.h"


UCLASS()
class UNDERFORGE_API ACarpentaryStation : public AForgeStation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarpentaryStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessMatItem(AForgeMat* material) override;
	virtual void ItemDectection(class AActor* ,bool entering) override;

	UFUNCTION(BlueprintCallable)
		void MorphStates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AHandlePart> StraightHandlePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AHandlePart> CurvedHandlePart;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EHandleType type);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh3;

	int CurrentState;
	bool PotentiallyInteracting;
};
