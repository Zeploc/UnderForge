// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utlities.generated.h"
/**
 * 
 */
#define ECC_Pickup		ECC_GameTraceChannel1
#define ECC_Enemy		ECC_GameTraceChannel2
#define ECC_Station		ECC_GameTraceChannel3

UENUM(BlueprintType)
enum class EResource : uint8
{
	R_WOOD UMETA(DisplayName = "Wood"),
	R_IRONORE UMETA(DisplayName = "Iron Ore"),
	R_COAL UMETA(DisplayName = "Coal"),
	R_COPPER UMETA(DisplayName = "Copper"),
	R_TIN UMETA(DisplayName = "Tin"),
	R_SILVER UMETA(DisplayName = "Silver"),
	R_GOLD UMETA(DisplayName = "Gold"),

	R_STEELINGOT UMETA(DisplayName = "Steel Ingot"),
	R_IRONINGOT UMETA(DisplayName = "Iron Ingot"),
	R_COPPERINGOT UMETA(DisplayName = "Copper Ingot"),
	R_TININGOT UMETA(DisplayName = "Tin Ingot"),
	R_BRONZEINGOT UMETA(DisplayName = "Bronze Ingot"),
	R_SILVERINGOT UMETA(DisplayName = "Silver Ingot"),
	R_GOLDINGOT UMETA(DisplayName = "Gold Ingot"),

	R_NONE UMETA(DisplayName = "Nothing"),
};

UENUM(BlueprintType)
enum class EBladeMat : uint8
{
	BM_STEEL UMETA(DisplayName = "Steel"),
	BM_IRON UMETA(DisplayName = "Iron"),

	BM_NONE UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EBladeType : uint8
{
	BT_BROADSWORD UMETA(DisplayName = "Broadsword Blade"),
	BT_KRIS UMETA(DisplayName = "Kris Blade"),
};

UENUM(BlueprintType)
enum class EHandleType : uint8
{
	HT_KRIS UMETA(DisplayName = "Kris Handle"),
	HT_BROADSWORD UMETA(DisplayName = "Broadsword Handle"),
};

UENUM(BlueprintType)
enum class EPartType : uint8
{
	PT_INGOT UMETA(DisplayName = "Ingot"),
	PT_BLADE UMETA(DisplayName = "Blade"),
	PT_HANDLE UMETA(DisplayName = "Handle"),
};

UENUM(BlueprintType)
enum class ESwordPart : uint8
{
	PT_NONE UMETA(DisplayName = "None"),
	PT_IRONKRISBLADE UMETA(DisplayName = "Iron Kris Blade"),
	PT_IRONBROADSWORDBLADE UMETA(DisplayName = "Iron BroadSword Blade"),
	PT_STEELKRISBLADE UMETA(DisplayName = "Steel Kris Blade"),
	PT_STEELBROADSWORDBLADE UMETA(DisplayName = "Steel BroadSword Blade"),
	PT_KRISHANDLE UMETA(DisplayName = "Kris Handle"),
	PT_BROADSWORDHANDLE UMETA(DisplayName = "Broadsword Handle"),
};


UENUM(BlueprintType)
enum class ETools : uint8
{
	T_NONE UMETA(DisplayName = "None"),
	T_HAMMER UMETA(DisplayName = "Hammer Tool"),
};


UENUM(BlueprintType)
enum class EEquipable : uint8
{
	PT_BRONZEKRISBLADE UMETA(DisplayName = "Bronze Kris Blade"),
};

UENUM(BlueprintType)
enum class EPickUpType : uint8
{
	PT_MAT UMETA(DisplayName = "Material"),
	PT_PART UMETA(DisplayName = "Part"),
	PT_WEAPON UMETA(DisplayName = "Weapon"),
	PT_TOOL UMETA(DisplayName = "Tool"),
};

UENUM(BlueprintType)
enum class EToolType : uint8
{
	TT_NONE UMETA(DisplayName = "None"),
	TT_HAMMER UMETA(DisplayName = "Hammer"),
	TT_CALIPERS UMETA(DisplayName = "Calipers"),
};


UENUM(BlueprintType)
enum class EWeaponPart : uint8
{
	WP_NONE UMETA(DisplayName = "None"),
	WP_BASICHANDLE UMETA(DisplayName = "Basic Handle"),
	WP_THREEQUARTERHANDLE UMETA(DisplayName = "Three Quarter Handle"),
	WP_DAGGERBLADE UMETA(DisplayName = "Dagger Blade"),
	WP_AXEHEAD UMETA(DisplayName = "Axe Head"),
	WP_BROADSWORDBLADE UMETA(DisplayName = "Broad Sword Blade"),
	WP_CUDGELHEAD UMETA(DisplayName = "Cudgel Head"),
	WP_RAPIERBLADE UMETA(DisplayName = "Rapier Blade"),
	WP_SPIKEDMACEHEAD UMETA(DisplayName = "Spiked Mace Head"),
};


USTRUCT(BlueprintType)
struct FIngotRecipe
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int iCoalCount = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<EResource> Resources;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fSmeltTime = 5.0f;

	bool operator==(FIngotRecipe other) {
		if (other.iCoalCount == iCoalCount && other.Resources == Resources && other.fSmeltTime == fSmeltTime)
			return true;
		else
			return false;
	}
};

USTRUCT(BlueprintType)
struct FWeaponPart
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMesh* PartMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTransform Offset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* IconImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* HorizontalIconImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class AForgePart> PartClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<EResource> ResourcesRequired;
};

USTRUCT(BlueprintType)
struct FWeapon
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<EWeaponPart> Parts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* IconImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* HorizontalIconImage;
};

USTRUCT(BlueprintType)
struct FResource
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* IconImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class APickUpItem> ResourceClass;


};

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	W_NONE UMETA(DisplayName = "None"),
	W_DAGGER UMETA(DisplayName = "Dagger"),
	W_BROADSWORD UMETA(DisplayName = "Broad Sword"),
	W_AXE UMETA(DisplayName = "Axe"),
	W_CUDGEL UMETA(DisplayName = "Cudgel"),
	W_RAPIER UMETA(DisplayName = "Rapier"),
	W_SPIKEDMACE UMETA(DisplayName = "Spiked Mace"),
};

UCLASS()
class UNDERFORGE_API UUtilities : public UObject
{
	GENERATED_BODY()

public:
	//static ESwordPart GetSwordPartEnum(class AForgePart* Part);
};
