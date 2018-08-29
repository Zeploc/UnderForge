// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgePart.h"
#include "Utlities.h"
#include "BladePart.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ABladePart : public AForgePart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EBladeMat BladeMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EBladeType BladeType;
	
};
