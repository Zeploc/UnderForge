// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgeItem.h"
#include "Utlities.h"
#include "SwordItem.generated.h"


/**
 * 
 */
UCLASS()
class UNDERFORGE_API ASwordItem : public AForgeItem
{
	GENERATED_BODY()

	ASwordItem();

public:
	bool CanHavePart(ESwordPart PartToCheck);
	bool AddPart(ESwordPart PartToAdd);

	UFUNCTION(BlueprintCallable)
		static bool IsHandle(ESwordPart SwordPart);
	UFUNCTION(BlueprintCallable)
		static bool IsBlade(ESwordPart SwordPart);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESwordPart, UStaticMesh*> PartMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* HandleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BladeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ESwordPart> ForgeParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* SuccessCombine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* FailureCombine;
private:

};
