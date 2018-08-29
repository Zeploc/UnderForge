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

	R_PROCESSEDWOOD UMETA(DisplayName = "ProcessedWood"),
	R_PROCESSEDWOOD2 UMETA(DisplayName = "ProcessedWood2"),
	R_STEEL UMETA(DisplayName = "Steel"),
	R_REALBRONZE UMETA(DisplayName = "RealBronze"),
};