// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "CraftingBench.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API ACraftingBench : public AForgeStation
{
	GENERATED_BODY()

public:

	ACraftingBench();

	virtual void ItemDectection(class AActor* OverlappActor, bool entering) override;
	virtual bool ProcessPartItem(AForgePart* Part) override;

	UFUNCTION(BlueprintCallable)
		void S_Disassemble();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASwordItem> SwordItem;

	UPROPERTY(BlueprintReadWrite, Replicated)
	class ASwordItem* CurrentItem;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* NewItemAddedSound;
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
		class USoundBase* DisassembledSound;

};
