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
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EResource Resource;
	
	UFUNCTION(BlueprintCallable)
		class AForgeMat* GetResource(AForgePlayer * _OwningPlayer);
	//UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
	//	void SERVER_GetResource();

	virtual void Interacted(class AForgePlayer* _Player) override;
};
