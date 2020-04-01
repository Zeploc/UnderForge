// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordItem.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Items/ForgeItem.h"
#include "CommonFunctions.h"
#include "UnderForgeSingleton.h"

#include "UnderForge.h"

ASwordItem::ASwordItem()
{
	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	HandleMesh->SetupAttachment(ItemMesh);
	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade Mesh"));
	BladeMesh->SetupAttachment(ItemMesh);
	HandleMesh->SetSimulatePhysics(false);
	BladeMesh->SetSimulatePhysics(false);
	HandleMesh->SetCollisionProfileName("OverlapAll");
	BladeMesh->SetCollisionProfileName("OverlapAll");

	ItemMesh->SetSimulatePhysics(false);
	//ItemMesh->SetVisibility(false);
	//ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BladeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	HandleMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundSuccess(TEXT("SoundWave'/Game/Sounds/SoundRourke/FinishCrafting_Sound.FinishCrafting_Sound'"));
	SuccessCombine = SoundSuccess.Object;

	AttachOffset.SetScale3D(FVector(0.3f));
	CanBePickedUp = false;
	iAttackDamage = 25;

	if (WeaponType != EWeapon::W_NONE)
	{
		SetUpWeapon(WeaponType);
	}
}

bool ASwordItem::CanHavePart(EWeaponPart PartToCheck)
{
	// if no parts are added, is valid
	if (ForgeParts.Num() <= 0)
	{
		return true;
	}

	// Check if part already exists in forgeparts
	for (EWeaponPart ForgePart : ForgeParts)
	{
		// Part already exists
		if (PartToCheck == ForgePart)
			return false;
	}


	// Checks
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return false;

	FWeapon* FoundWeapon = GameSingleton->Weapons.Find(WeaponType);
	if (!FoundWeapon)
		return false;


	// Check if part is in current weapon
	for (EWeaponPart WeaponPart : FoundWeapon->Parts)
	{
		// Part is in required weapon parts, so is valid part
		if (PartToCheck == WeaponPart)
			return true;
	}

	bool bCanHaveWeapon = false;
	// Check through weapons to see if this part matches a weapon config using existing parts
	TArray<EWeapon> Weapons;
	GameSingleton->Weapons.GenerateKeyArray(Weapons);
	for (EWeapon Weapon : Weapons)
	{
		// Create required for this weapon
		TArray<EWeaponPart> PartsRequired = GameSingleton->Weapons[Weapon].Parts;
		for (EWeaponPart CurrentPart : ForgeParts)
		{
			// Removes current part from required
			PartsRequired.RemoveSingle(CurrentPart);
		}
		// If required parts still contains given part
		if (PartsRequired.Contains(PartToCheck))
		{
			bCanHaveWeapon = true;
			WeaponType = Weapon;
		}
	}

	return bCanHaveWeapon;

	/*for (int i = 0; i < ForgeParts.Num(); i++)
	{
		if (UCommonFunctions::IsHandle(ForgeParts[i]) && UCommonFunctions::IsHandle(PartToCheck))
		{
			return false;
		}
		else if (UCommonFunctions::IsBlade(ForgeParts[i]) && UCommonFunctions::IsBlade(PartToCheck))
		{
			return false;
		}
	}
	return true;*/
}

bool ASwordItem::AddPart(EWeaponPart PartToAdd)
{
	if (HasAuthority())
	{
		MULTI_AddPart(PartToAdd);
	}

	// If forge parts empty, set current weapon to found
	bool FirstPart = false;
	if (ForgeParts.Num() <= 0)
		FirstPart = true;

	// Add part and mesh
	ForgeParts.Add(PartToAdd); // Add the part
	AddPartMesh(PartToAdd, FName(TEXT("Part %d"), ForgeParts.Num()));

	// Checks
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return false;

	// First part, so set weapon type
	if (FirstPart)
	{
		// Get all weapons
		TArray<EWeapon> Weapons;
		GameSingleton->Weapons.GenerateKeyArray(Weapons);
		for (EWeapon Weapon : Weapons)
		{
			// Find weapon's required parts
			FWeapon* FoundWeapon = GameSingleton->Weapons.Find(Weapon);
			if (FoundWeapon)
			{
				// Check if the part is contained in this weapon's parts
				for (EWeaponPart WeaponPart : FoundWeapon->Parts)
				{
					// Part is in this weapon parts
					if (WeaponPart == PartToAdd)
					{
						// Set current weapon and break
						WeaponType = Weapon;
						break;
					}
				}
			}
			// Weapon has been found, exit loop check
			if (WeaponType != EWeapon::W_NONE)
				break;
		}
	}
	
	FWeapon* FoundWeapon = GameSingleton->Weapons.Find(WeaponType);
	if (!FoundWeapon)
		return false;

	// Check through each parts of the weapon
	for (EWeaponPart WeaponPart : FoundWeapon->Parts)
	{
		// If part not contained, weapon not complete
		if (!ForgeParts.Contains(WeaponPart))
			return false;
	}
	ItemMesh->SetSimulatePhysics(true);
	UGameplayStatics::PlaySound2D(GetWorld(), SuccessCombine);

	return true;

	//bool HasHandle = false;
	//bool HadBlade = false;

	//for (int i = 0; i < ForgeParts.Num(); i++)
	//{
	//	if (UCommonFunctions::IsHandle(ForgeParts[i]))
	//	{
	//		HasHandle = true;
	//		HandleMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
	//	}
	//	else if (UCommonFunctions::IsBlade(ForgeParts[i]))
	//	{
	//		HadBlade = true;
	//		BladeMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
	//	}
	//}

	//if (HasHandle && HadBlade)
	//{
	//	ItemMesh->SetSimulatePhysics(true);
	//	UGameplayStatics::PlaySound2D(GetWorld(), SuccessCombine);
	//	//ItemMesh->SetVisibility(true);
	//	//HandleMesh->SetVisibility(false);
	//	//BladeMesh->SetVisibility(false);
	//	return true;
	//}
	//return false;
}
void ASwordItem::MULTI_AddPart_Implementation(EWeaponPart PartToAdd)
{
	if (!HasAuthority())
	{
		AddPart(PartToAdd);
	}
}

void ASwordItem::SetUpWeapon(EWeapon _Weapon)
{
	
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
		return;

	FWeapon* FoundWeapon = GameSingleton->Weapons.Find(_Weapon);
	if (!FoundWeapon)
		return;

	ClearCurrentParts();

	int PartID = 1;
	for (EWeaponPart WeaponPart : FoundWeapon->Parts)
	{
		FName PartName = FName(TEXT("Part %d"), PartID);
		AddPartMesh(WeaponPart, PartName);
		PartID++;
	}
}

void ASwordItem::AddPartMesh(EWeaponPart WeaponPart, const FName PartName)
{
	UUnderForgeSingleton* GameSingleton = Cast<UUnderForgeSingleton>(GEngine->GameSingleton);
	FWeaponPart* FoundWeaponPart = GameSingleton->Parts.Find(WeaponPart);
	if (!FoundWeaponPart)
		return;

	UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(this, PartName);
	if (!NewMeshComp)
		return;

	if (FoundWeaponPart->PartMesh)
		NewMeshComp->SetStaticMesh(FoundWeaponPart->PartMesh);
	NewMeshComp->SetCollisionProfileName("NoCollision");
	NewMeshComp->RegisterComponent();

	NewMeshComp->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	NewMeshComp->SetRelativeTransform(FoundWeaponPart->Offset);
	PartComponents.Add(WeaponPart, NewMeshComp);
}

void ASwordItem::ClearCurrentParts()
{
	TArray<UStaticMeshComponent*> Comps;
	PartComponents.GenerateValueArray(Comps);
	for (UStaticMeshComponent* StaticMeshComp : Comps)
	{
		StaticMeshComp->DestroyComponent();
	}
	PartComponents.Empty();
}




void ASwordItem::Disassemble()
{
	WeaponType = EWeapon::W_NONE;
	for (int i = 0; i < ForgeParts.Num(); i++)
	{
		//MakeResource(ForgeParts[i]);
	}
	Destroy();
}

//AForgePart * ASwordItem::MakeResource(ESwordPart type)
//{
//	switch (type)
//	{
//		case ESwordPart::PT_BROADSWORDHANDLE:
//		{
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(BroadswordHandle, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//		case ESwordPart::PT_KRISHANDLE:
//		{
//
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(KrisHandle, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//		case ESwordPart::PT_IRONBROADSWORDBLADE:
//		{
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronBroadBladePart, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//		case ESwordPart::PT_IRONKRISBLADE:
//		{
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronKrisBladePart, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//		case ESwordPart::PT_STEELBROADSWORDBLADE:
//		{
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(SteelBroadBladePart, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//		case ESwordPart::PT_STEELKRISBLADE:
//		{
//			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(SteelKrisBladePart, GetActorLocation(), GetActorRotation());
//			return ResourceRef;
//			break;
//		}
//	}
//	return nullptr;
//}