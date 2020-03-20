// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgePart.h"
#include "Utlities.h"
#include "HandlePart.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AHandlePart : public AForgePart
{
	GENERATED_BODY()

		AHandlePart();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EHandleType HandleType;
	
	
};
