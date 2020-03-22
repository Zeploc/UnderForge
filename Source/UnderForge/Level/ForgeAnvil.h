// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "ForgeAnvil.generated.h"


class AForgePlayer;
/**
 * 
 */
UCLASS()
class UNDERFORGE_API AForgeAnvil : public AForgeStation
{
	GENERATED_BODY()

protected:
	//sound
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	class USoundBase* Success;
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	class USoundBase* Failure;
	//---------
	AForgeAnvil();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void ProcessPartItem(AForgePart* Part) override;

	UFUNCTION(BlueprintCallable)
		void MorphStates(bool Next);

	void HammeringMinigame(float Deltatime);
	void HammeringCycle(class AForgePlayer* Player);

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EBladeMat type);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SERVER_MakeResource(EBladeMat type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelKrisBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelBroadBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronKrisBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronBroadBladePart;
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* BroadswordBlade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* KrisSwordBlade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBladeType CurrentState;
	bool PotentiallyInteracting;
	
	FTimerHandle SuccessHitTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MarkerMoveSpeed = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeMin = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeMax = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxCycles = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)//, Replicated)
		float CurrentMinRange = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)//, Replicated)
		float CurrentMaxRange = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)//, Replicated)
		float CurrentMarkerPos = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PauseTimeOnSuccess = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool SuccessHit = false;
	
	int HammingCycles = 0;

	float CurrentMarkerMoveSpeed = 0.8f;
	void SuccessTimeComplete();
	void RandomiseRange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBladeMat CurrentResource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHammerMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float HammerTimePassed;
	EResource CurrentlyProcessing;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OutputName;
};
