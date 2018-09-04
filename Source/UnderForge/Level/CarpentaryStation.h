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

	UFUNCTION(BlueprintCallable)
		void MorphStates(bool Next);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> BroadswordHandlePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> KrisHandlePart;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EHandleType type);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* BroadSwordHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* KrisSwordHandle;

	EHandleType CurrentState;
	bool PotentiallyInteracting;


	//Spining minigame Section
	float CurrentX;
	float CurrentY;
	float PreviousX;
	float PreviousY;

	UFUNCTION(BlueprintCallable)
		void SetXValue(float x);
	UFUNCTION(BlueprintCallable)
		void SetYValue(float y);
	UFUNCTION(BlueprintCallable)
		void BeginMinigame();
	UFUNCTION(BlueprintCallable)
		void SpinningMinigame();

	bool bSpinningGamePlaying;
	float SpinningTotal;
};
