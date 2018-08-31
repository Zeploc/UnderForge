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
	float SmeltingTimeNeeded;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SmeltingTimeKABOOM;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessMatItem(AForgeMat* material) override;
	virtual void ItemDectection(class AActor*, bool entering) override;

	void SmeltingMinigame(float DeltaTime);
	void MiniGameComplete();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronIngotPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelForgedPart;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgeMat> IronMat;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
	UFUNCTION(BlueprintCallable)
		class AForgeMat* MakeMat(EResource type);

	bool bSmeltingMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float SmeltingTimePassed;
	EResource CurrentlyProcessing;
	FTimerHandle timer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentRemainingTime = 0.0f;
};
