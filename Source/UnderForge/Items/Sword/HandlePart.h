// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgePart.h"
#include "HandlePart.generated.h"

UENUM(BlueprintType)
enum class EHandleType : uint8
{
	HT_STRAIGHT UMETA(DisplayName = "Straight"),
	HT_CURVED UMETA(DisplayName = "Curved"),
};
/**
 * 
 */
UCLASS()
class UNDERFORGE_API AHandlePart : public AForgePart
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EHandleType HandleType;
	
	
};
