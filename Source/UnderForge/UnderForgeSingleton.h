// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Utlities.h"

#include "UnderForgeSingleton.generated.h"
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNDERFORGE_API UUnderForgeSingleton : public UObject
{
	GENERATED_BODY()

public:
	UUnderForgeSingleton(const FObjectInitializer& ObjectInitializer);

protected:


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TMap<EWeapon, FWeapon> Weapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TMap<EWeaponPart, FWeaponPart> Parts;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TMap<EResource, FResource> Resources;

	

protected:

};
