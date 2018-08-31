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
	AForgeAnvil();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void ItemDectection(class AActor* OverlappActor, bool entering) override;
	virtual void ProcessPartItem(AForgePart* Part) override;
	void MorphStates();

	void HammeringMinigame(float Deltatime);
	void HammeringCycle();
	void RandomizeHammeringWindow();

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EBladeMat type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelKrisBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelBroadBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronKrisBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronBroadBladePart;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh2;



	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* StraightSwordBlade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* KrisSwordBlade;



	int CurrentState;
	bool PotentiallyInteracting;

	float HammerTimeMax;
	float HammerTimeNeeded;
	float HammerTimeKABOOM;
	int HammingCycles;
	int MaxCycles;

	EBladeMat CurrentResource;
	bool bHammerMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float HammerTimePassed;
	EResource CurrentlyProcessing;
};
