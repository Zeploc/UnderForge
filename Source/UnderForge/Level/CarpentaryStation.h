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
	virtual bool ProcessMatItem(AForgeMat* material) override;

	virtual void Interacted(class AForgePlayer* _Player) override;

	UFUNCTION(Server, WithValidation, Reliable)
		void SERVER_SetCurrent(float _x, float _y);
	UFUNCTION(NetMulticast, Reliable)
		void MULTI_SetCurrent(float _x, float _y);

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SERVER_MakeResource(EResource type);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EResource CurrentResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AForgePlayer * CurrentPlayer;


	//Spining minigame Section
	float CurrentX;
	float CurrentY;
	float PreviousX;
	float PreviousY;

	//UFUNCTION(BlueprintCallable)
	//	void SetXValue(float x);
	//UFUNCTION(BlueprintCallable)
	//	void SetYValue(float y);

	UFUNCTION(BlueprintCallable)
		void BeginMinigame();
	UFUNCTION(BlueprintCallable)
		void SpinningMinigame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSpinningGamePlaying;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpinningTotal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpinningRequired;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OutputName;
};
