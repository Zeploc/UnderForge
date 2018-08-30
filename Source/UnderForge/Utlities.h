// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
};


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
