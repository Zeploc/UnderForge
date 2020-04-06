// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ForgePlayer.generated.h"

UCLASS()
class UNDERFORGE_API AForgePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AForgePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called for side to side input */
	void MoveUp(float Val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns true if interacted (With station)
	UFUNCTION(BlueprintCallable)
		bool Interact();

	UFUNCTION(Server, WithValidation, Reliable)
		void SERVER_InteractWith(class AForgeStation* Station);

	UFUNCTION(Client, Reliable)
		void CLIENT_PickUp(class APickUpItem* PickUp);

	UFUNCTION(BlueprintCallable)
		void ClearHoldItem();
	UFUNCTION(BlueprintCallable)
		void LocalClearHoldItem();
	UFUNCTION(Server, WithValidation, Reliable)
		void SERVER_ClearHoldItem();
	UFUNCTION(NetMulticast, Reliable)
		void MULTI_ClearHoldItem();

	APickUpItem* GetHoldItem() { return HoldItem;  };

	UFUNCTION(BlueprintCallable)
	void SetHoldItem(class APickUpItem* PickUp);
	UFUNCTION(Server, WithValidation, Reliable)
		void SERVER_SetHoldItem(class APickUpItem* PickUp);
	UFUNCTION(NetMulticast, Reliable)
		void MULTI_SetHoldItem(class APickUpItem* PickUp);


	UFUNCTION(BlueprintCallable)
		void SecondaryInteract();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractRange = 100.0f;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartAttack();

	UFUNCTION(BlueprintCallable)
		void LockPosition(bool IsLocked);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack3;


	UFUNCTION(BlueprintPure)
		bool HoldingWeapon();
	UFUNCTION(BlueprintPure)
		bool HoldingHammer();
		
	//TEST

	UFUNCTION(BlueprintCallable)
		void SetXValue(float x);
	UFUNCTION(BlueprintCallable)
		void SetYValue(float y);

	UPROPERTY(BlueprintReadOnly)
	float CurrentX;
	UPROPERTY(BlueprintReadOnly)
	float CurrentY;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APickUpItem* HoldItem;
};
