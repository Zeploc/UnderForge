// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utlities.generated.h"
/**
 * 
 */
#define ECC_Pickup		ECC_GameTraceChannel1
#define ECC_Enemy		ECC_GameTraceChannel2
#define ECC_Station		ECC_GameTraceChannel3

UENUM(BlueprintType)
enum class EResource : uint8
{
	R_WOOD UMETA(DisplayName = "Wood"),
	R_IRONORE UMETA(DisplayName = "Iron Ore"),

	R_STEELINGOT UMETA(DisplayName = "Steel Ingot"),
	R_IRONINGOT UMETA(DisplayName = "Iron Ingot"),


	R_NONE UMETA(DisplayName = "Nothing"),
};

UENUM(BlueprintType)
enum class EBladeMat : uint8
{
	BM_STEEL UMETA(DisplayName = "Steel"),
	BM_IRON UMETA(DisplayName = "Iron"),

	BM_NONE UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EBladeType : uint8
{
	BT_BROADSWORD UMETA(DisplayName = "Broadsword Blade"),
	BT_KRIS UMETA(DisplayName = "Kris Blade"),
};

UENUM(BlueprintType)
enum class EHandleType : uint8
{
	HT_KRIS UMETA(DisplayName = "Kris Handle"),
	HT_BROADSWORD UMETA(DisplayName = "Broadsword Handle"),
};

UENUM(BlueprintType)
enum class EPartType : uint8
{
	PT_INGOT UMETA(DisplayName = "Ingot"),
	PT_BLADE UMETA(DisplayName = "Blade"),
	PT_HANDLE UMETA(DisplayName = "Handle"),
};

UENUM(BlueprintType)
enum class ESwordPart : uint8
{
	PT_NONE UMETA(DisplayName = "None"),
	PT_IRONKRISBLADE UMETA(DisplayName = "Iron Kris Blade"),
	PT_IRONBROADSWORDBLADE UMETA(DisplayName = "Iron BroadSword Blade"),
	PT_STEELKRISBLADE UMETA(DisplayName = "Steel Kris Blade"),
	PT_STEELBROADSWORDBLADE UMETA(DisplayName = "Steel BroadSword Blade"),
	PT_KRISHANDLE UMETA(DisplayName = "Kris Handle"),
	PT_BROADSWORDHANDLE UMETA(DisplayName = "Broadsword Handle"),
};


UENUM(BlueprintType)
enum class ETools : uint8
{
	T_NONE UMETA(DisplayName = "None"),
	T_HAMMER UMETA(DisplayName = "Hammer Tool"),
};


UENUM(BlueprintType)
enum class EEquipable : uint8
{
	PT_BRONZEKRISBLADE UMETA(DisplayName = "Bronze Kris Blade"),
};

UCLASS()
class UNDERFORGE_API UUtilities : public UObject
{
	GENERATED_BODY()

public:
	static ESwordPart GetSwordPartEnum(class AForgePart* Part);
};
