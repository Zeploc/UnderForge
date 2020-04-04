// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Items/ForgeMat.h"
#include "ResourcePile.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AResourcePile : public AForgeStation
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EResource Resource;

	void RecreateResource();

	UFUNCTION(BlueprintCallable)
	class AForgeMat* GetResource();
	//UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
	//	void SERVER_GetResource();

	virtual bool CanTakeItem(class APickUpItem* Item) override;
	virtual void ItemPickedUp(class APickUpItem* Item) override;

	virtual void Interacted(class AForgePlayer* _Player) override;
};
