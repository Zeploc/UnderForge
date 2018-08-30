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

	void HammeringMinigame(float Deltatime);
	void HammeringCycle();

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EBladeMat type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> BronzeBladePart;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* StationMesh2;

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
