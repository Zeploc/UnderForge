// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordItem.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

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
}

bool ASwordItem::CanHavePart(ESwordPart PartToCheck)
{
	for (int i = 0; i < ForgeParts.Num(); i++)
	{
		if (IsHandle(ForgeParts[i]) && IsHandle(PartToCheck))
		{
			return false;
		}
		else if (IsBlade(ForgeParts[i]) && IsBlade(PartToCheck))
		{
			return false;
		}
	}
	return true;
}

bool ASwordItem::AddPart(ESwordPart PartToAdd)
{
	ForgeParts.Add(PartToAdd); // Add the part

	bool HasHandle = false;
	bool HadBlade = false;

	for (int i = 0; i < ForgeParts.Num(); i++)
	{
		if (IsHandle(ForgeParts[i]))
		{
			HasHandle = true;
			HandleMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
		}
		else if (IsBlade(ForgeParts[i]))
		{
			HadBlade = true;
			BladeMesh->SetStaticMesh(PartMeshes[ForgeParts[i]]);
		}
	}

	if (HasHandle && HadBlade)
	{
		ItemMesh->SetSimulatePhysics(true);
		//ItemMesh->SetVisibility(true);
		//HandleMesh->SetVisibility(false);
		//BladeMesh->SetVisibility(false);
		return true;
	}
	return false;
}

bool ASwordItem::IsHandle(ESwordPart SwordPart)
{
	if (SwordPart == ESwordPart::PT_BROADSWORDHANDLE ||
		SwordPart == ESwordPart::PT_KRISHANDLE)
	{
		return true;
	}

	return false;
}

bool ASwordItem::IsBlade(ESwordPart SwordPart)
{
	if (SwordPart == ESwordPart::PT_IRONBROADSWORDBLADE ||
		SwordPart == ESwordPart::PT_IRONKRISBLADE ||
		SwordPart == ESwordPart::PT_STEELBROADSWORDBLADE ||
		SwordPart == ESwordPart::PT_STEELKRISBLADE)
	{
		return true;
	}
	return false;
}
