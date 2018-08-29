// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "ResourcePile.generated.h"

UENUM(BlueprintType)
enum class EResource : uint8
{
	R_WOOD UMETA(DisplayName = "Wood"),
	R_IRON UMETA(DisplayName = "Iron"),
	R_BRONZE UMETA(DisplayName = "Bronze"),
};
/**
 * 
 */
UCLASS()
class UNDERFORGE_API AResourcePile : public AForgeStation
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EResource Resource;
	
};
