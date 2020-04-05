// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ForgeItem.h"
#include "Utlities.h"
#include "SwordItem.generated.h"

//class AForgePart;
/**
 * 
 */
UCLASS()
class UNDERFORGE_API ASwordItem : public AForgeItem
{
	GENERATED_BODY()

public:
	ASwordItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	bool CanHavePart(EWeaponPart PartToCheck);
	bool AddPart(EWeaponPart PartToAdd);

	UFUNCTION(NetMulticast, Reliable)
	void MULTI_AddPart(EWeaponPart PartToAdd);

	void SetUpWeapon(EWeapon _Weapon);
	void AddPartMesh(EWeaponPart WeaponPart, const FName PartName);
	void UpdateAltPartMeshes();
	void ClearCurrentParts();

	void SetWeaponType(EWeapon _Weapon);
	UFUNCTION(BlueprintImplementableEvent)
	void BI_OnSetWeaponType(EWeapon _Weapon);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EWeaponPart, UStaticMeshComponent*> PartComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EWeaponPart> ForgeParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* SuccessCombine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* FailureCombine;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		EWeapon WeaponType = EWeapon::W_NONE;

	UPROPERTY(BlueprintReadWrite)
		FWeapon CurentWeaponStats;

	UFUNCTION(BlueprintCallable)
		void Disassemble();

private:

};
