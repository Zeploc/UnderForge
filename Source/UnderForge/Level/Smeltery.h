// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "Smeltery.generated.h"


UCLASS()
class UNDERFORGE_API ASmeltery : public AForgeStation
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmeltery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	float SmeltingTimeMax;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SmeltingTimeNeeded;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SmeltingTimeKABOOM;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessMatItem(AForgeMat* material) override;

	void SmeltingMinigame(float DeltaTime);
	void MiniGameComplete();
	UFUNCTION(BlueprintCallable)
		void MorphStates(bool Next);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronIngotPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelIngotPart;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgeMat> IronMat;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
	UFUNCTION(BlueprintCallable)
		class AForgeMat* MakeMat(EResource type);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSmeltingMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float SmeltingTimePassed;
	EResource CurrentlyProcessing;
	FTimerHandle timer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentRemainingTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float ForgeHealth = 1.0f;

	UFUNCTION(BlueprintCallable)
		void DamageForge(float Damage);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* IronIngot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* SteelIngot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EResource CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OutputName;

};
