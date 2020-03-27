// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utlities.h"
#include "PickUpItem.generated.h"

UCLASS()
class UNDERFORGE_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* ItemMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Drop();

	UFUNCTION(BlueprintCallable)
		virtual void PickUp(class AForgePlayer* NewPlayer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanBePickedUp = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AForgePlayer* HeldPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AForgeStation* CurrentTouchingStation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPickUpType PickUpType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EToolType ToolType = EToolType::TT_NONE;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int iAttackDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fKnockbackMultiplier = 1.0f;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FTransform AttachOffset;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName SocketName = "MiddleFinger2_R";


public:



};
