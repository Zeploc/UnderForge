// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utlities.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EResource : uint8
{
	R_WOOD UMETA(DisplayName = "Wood"),
	R_IRON UMETA(DisplayName = "Iron"),
	R_BRONZE UMETA(DisplayName = "Bronze"),

	R_STEEL UMETA(DisplayName = "Steel"),
	R_REALBRONZE UMETA(DisplayName = "RealBronze"),


	R_NONE UMETA(DisplayName = "Nothing"),
};

UENUM(BlueprintType)
enum class EBladeMat : uint8
{
	BM_BRONZE UMETA(DisplayName = "Bronze"),
	BM_IRON UMETA(DisplayName = "Iron"),

	BM_NONE UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EBladeType : uint8
{
	BT_BROADSWORD UMETA(DisplayName = "Broadsword"),
	BT_KRIS UMETA(DisplayName = "Kris"),
};

UENUM(BlueprintType)
enum class EHandleType : uint8
{
	HT_STRAIGHT UMETA(DisplayName = "Straight"),
	HT_CURVED UMETA(DisplayName = "Curved"),
};

UENUM(BlueprintType)
enum class EPartType : uint8
{
	PT_BLADE UMETA(DisplayName = "Blade"),
	PT_UNFORMEDBLADE UMETA(DisplayName = "Unformed Blade"),
	PT_HANDLE UMETA(DisplayName = "Handle"),
};

UENUM(BlueprintType)
enum class ESwordPart : uint8
{
	PT_BRONZEKRISBLADE UMETA(DisplayName = "Bronze Kris Blade"),
	PT_BRONZEBROADSWORDBLADE UMETA(DisplayName = "Bronze BroadSword Blade"),
	PT_STEELKRISBLADE UMETA(DisplayName = "Steel Kris Blade"),
	PT_STEELBROADSWORDBLADE UMETA(DisplayName = "Steel BroadSword Blade"),
	PT_CURVEDHANDLE UMETA(DisplayName = "Curved Handle"),
	PT_STRAIGHTHANDLE UMETA(DisplayName = "Straight Handle"),
};

UCLASS()
class UNDERFORGE_API UUtilities : public UObject
{
	GENERATED_BODY()

public:
	static ESwordPart GetSwordPartEnum(class AForgePart* Part);
};
