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

	bool IsHandle(ESwordPart SwordPart);
	bool IsBlade(ESwordPart SwordPart);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESwordPart, UStaticMesh*> PartMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* HandleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BladeMesh;

private:
	UPROPERTY(EditAnywhere)
		TArray<ESwordPart> ForgeParts;

};
