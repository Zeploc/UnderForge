// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgeItem.h"
#include "SwordItem.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ASwordItem : public AForgeItem
{
	GENERATED_BODY()
	

	UFUNCTION(BlueprintCallable)
		virtual void AddPart(class AForgePart* ForgePart) override;	
	
};
