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
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* FullyCompletedCrafting;
	//---------
	AForgeAnvil();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void ProcessPartItem(AForgePart* Part) override;

	virtual bool TryInteract(class AForgePlayer* _Player) override;
	//virtual void Interacted(class AForgePlayer* _Player) override;

	UFUNCTION(BlueprintCallable)
	void HitWithHammer(class AForgePlayer* _Player);
	//virtual void MorphStates(bool Next) override;

	UFUNCTION(Server, reliable, WithValidation)
		void SERVER_InteractHit(bool _Success);
	UFUNCTION(NetMulticast, reliable)
		void MULTI_InteractHit(bool _Success);

	void HammeringMinigame(float Deltatime);

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SERVER_MakeResource(EResource type);

		UFUNCTION(BlueprintPure)
	static EWeaponPart FindPartFromResource(EResource _Resource);
			

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;
	
	
	bool PotentiallyInteracting;
	
	FTimerHandle SuccessHitTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MarkerMoveSpeed = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeMin = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeMax = 0.6f;
	UPROPERTY(BlueprintReadWrite)
		int CurrentOrb = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int OrbCount = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		float CurrentMinRange = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
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
		EResource CurrentResource;

	UPROPERTY(EditDefaultsOnly)
	TArray<EResource> AvailableResourceTypes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHammerMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float HammerTimePassed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OutputName;
};
