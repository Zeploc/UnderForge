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
	float CurrentRecipeSmeltingTimeNeeded;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SmeltingTimeKABOOM;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryInteract(class AForgePlayer* _Player) override;
	virtual void Interacted(class AForgePlayer* _Player) override;


	virtual bool CanTakeMatItem(class AForgeMat* material);
	virtual bool ProcessMatItem(AForgeMat* material) override;


	UFUNCTION(Server, reliable, WithValidation)
		void SERVER_ChangeCurrentRecipe(FIngotRecipe _NewRecipe);
	UFUNCTION(NetMulticast, reliable)
		void MULTI_ChangeCurrentRecipe(FIngotRecipe _NewRecipe);

	UFUNCTION(Server, reliable, WithValidation)
		void SERVER_ClearCurrentRecipe();
	UFUNCTION(NetMulticast, reliable)
		void MULTI_ClearCurrentRecipe();

	UFUNCTION(Server, reliable, WithValidation)
		void SERVER_ChangeResource(EResource _ResourceChange, bool _Add = true);
	UFUNCTION(NetMulticast, reliable)
		void MULTI_ChangeResource(EResource _ResourceChange, bool _Add = true);

	UFUNCTION(BlueprintImplementableEvent)
		void BI_OnNewResource(EResource _Resource);
	UFUNCTION(BlueprintImplementableEvent)
		void BI_OnRemoveResource(EResource _Resource);

	void ProcessSmelting(float DeltaTime);

	bool CanHaveResource(EResource _Resource, FIngotRecipe& FoundRecipe);
	TArray<EResource> GetRequiredForRecipe(FIngotRecipe Recipie);
	EResource GetResourceFromRecipe(FIngotRecipe _IngotRecipe);
	bool IsCompletedRecipe(FIngotRecipe _Recipe);
	bool FindCompletedRecipe(FIngotRecipe& FoundRecipe);
	//virtual void MorphStates(bool Next) override;
	
	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EResource type);
	UFUNCTION(BlueprintCallable)
		class AForgeMat* MakeMat(EResource type);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SERVER_MakeResource(EResource type);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void SERVER_MakeMat(EResource type);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSmeltingMinigamePlaying;
	UPROPERTY(BlueprintReadWrite)
		float SmeltingTimePassed;

	UPROPERTY(BlueprintReadWrite)//, Replicated)
	 TArray<EResource> CurrentlyProcessing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EResource, FIngotRecipe> IngotRecipies;

	FIngotRecipe* CurrentRecipe;

	UPROPERTY(BlueprintReadWrite)
	EResource CurrentResourceCreating = EResource::R_NONE;

	FTimerHandle timer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentRemainingTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float ForgeHealth = 1.0f;

	UFUNCTION(BlueprintCallable)
		void DamageForge(float Damage);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void MULTI_UpdateForgeHealth(float _NewHealth);
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* SmelteryTakeDamageSound;

	UFUNCTION(BlueprintCallable)
		void RestartHealth();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* CurrentProducingItem;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* Rotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EResource CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OutputName;

};
