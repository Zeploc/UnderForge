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
	ItemMesh->SetVisibility(false);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BladeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	HandleMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundSuccess(TEXT("SoundWave'/Game/Sounds/SoundRourke/FinishCrafting_Sound.FinishCrafting_Sound'"));
	SuccessCombine = SoundSuccess.Object;

	AttachOffset.SetScale3D(FVector(0.3f));
	CanBePickedUp = false;
	iAttackDamage = 25;
}

bool ASwordItem::CanHavePart(ESwordPart PartToCheck)
{
	for (int i = 0; i < ForgeParts.Num(); i++)
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
	return true;
}

bool ASwordItem::AddPart(ESwordPart PartToAdd)
{
	if (HasAuthority())
	{
		MULTI_AddPart(PartToAdd);
	}
	ForgeParts.Add(PartToAdd); // Add the part

	bool HasHandle = false;
	bool HadBlade = false;

	for (int i = 0; i < ForgeParts.Num(); i++)
	{
		if (UCommonFunctions::IsHandle(ForgeParts[i]))
		{
			HasHandle = true;
			HandleMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
		}
		else if (UCommonFunctions::IsBlade(ForgeParts[i]))
		{
			HadBlade = true;
			BladeMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
		}
	}

	if (HasHandle && HadBlade)
	{
		ItemMesh->SetSimulatePhysics(true);
		UGameplayStatics::PlaySound2D(GetWorld(), SuccessCombine);
		//ItemMesh->SetVisibility(true);
		//HandleMesh->SetVisibility(false);
		//BladeMesh->SetVisibility(false);
		return true;
	}
	return false;
}

void ASwordItem::MULTI_AddPart_Implementation(ESwordPart PartToAdd)
{
	if (!HasAuthority())
	{
		AddPart(PartToAdd);
	}
}



void ASwordItem::Disassemble()
{
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