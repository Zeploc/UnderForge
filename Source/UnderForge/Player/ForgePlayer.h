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

	UFUNCTION(BlueprintCallable)
		void Interact();
	UFUNCTION(BlueprintCallable)
		void SecondaryInteract();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractRange = 100.0f;
	

};
