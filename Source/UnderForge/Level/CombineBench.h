// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/ForgeStation.h"
#include "Utlities.h"
#include "CombineBench.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API ACombineBench : public AForgeStation
{
	GENERATED_BODY()

public:

	ACombineBench();

	virtual void ItemDectection(class AActor* OverlappActor, bool entering) override;
	virtual void ProcessPartItem(AForgePart* Part) override;

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
