// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgePart.h"
#include "BladePart.generated.h"

UENUM(BlueprintType)
enum class EBladeMat : uint8
{
	BM_BRONZE UMETA(DisplayName = "Bronze"),
	BM_IRON UMETA(DisplayName = "Iron"),
};

UENUM(BlueprintType)
enum class EBladeType : uint8
{
	BT_BROADSWORD UMETA(DisplayName = "Broadsword"),
	BT_KRIS UMETA(DisplayName = "Kris"),
};

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
