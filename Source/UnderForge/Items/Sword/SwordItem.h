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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* HandleMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* BladeMesh;

public:
	bool CanHavePart(EWeaponPart PartToCheck);
	bool AddPart(EWeaponPart PartToAdd);

	UFUNCTION(NetMulticast, Reliable)
	void MULTI_AddPart(EWeaponPart PartToAdd);

	void SetUpWeapon(EWeapon _Weapon);
	void AddPartMesh(EWeaponPart WeaponPart, const FName PartName);
	void ClearCurrentParts();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESwordPart, UStaticMesh*> PartMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EWeaponPart, UStaticMeshComponent*> PartComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EWeaponPart> ForgeParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* SuccessCombine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* FailureCombine;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> BroadswordHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> KrisHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronBroadBladePart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> IronKrisBladePart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelBroadBladePart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AForgePart> SteelKrisBladePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeapon WeaponType = EWeapon::W_NONE;

	UFUNCTION(BlueprintCallable)
		void Disassemble();

	//UFUNCTION(BlueprintCallable)
		//class AForgePart* MakeResource(ESwordPart type);
private:

};
