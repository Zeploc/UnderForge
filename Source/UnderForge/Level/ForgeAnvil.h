// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "ForgeAnvil.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API AForgeAnvil : public AForgeStation
{
	GENERATED_BODY()
	
public:
	virtual void ItemDectection(class AActor* OverlappActor, bool entering) override;
	virtual void ProcessPartItem(AForgePart* Part) override;

	UFUNCTION(BlueprintCallable)
		class AForgePart* MakeResource(EBladeMat type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> BronzeBladePart;
};
