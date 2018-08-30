// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgeItem.h"
#include "Utlities.h"
#include "SwordItem.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ASwordItem : public AForgeItem
{
	GENERATED_BODY()
	

public:
	bool CanHavePart(ESwordPart PartToCheck);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ESwordPart> ForgeParts;

	bool IsHandle(ESwordPart SwordPart);
	bool IsBlade(ESwordPart SwordPart);
};
