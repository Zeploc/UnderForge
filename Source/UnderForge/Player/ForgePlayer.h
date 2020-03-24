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
		void SecondaryInteract();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractRange = 100.0f;

	UFUNCTION(BlueprintCallable)
		void LockPosition(bool IsLocked);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Attack3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APickUpItem* HoldItem;

	UFUNCTION(BlueprintPure)
		bool HoldingWeapon();
	UFUNCTION(BlueprintPure)
		bool HoldingHammer();
		
	//TEST
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACarpentaryStation* currentLathe;
	UFUNCTION(BlueprintCallable)
		void latheReset();

	UFUNCTION(BlueprintCallable)
		void SetXValue(float x);
	UFUNCTION(BlueprintCallable)
		void SetYValue(float y);
	float CurrentX;
	float CurrentY;
};
