// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"

// TEMP
#include "Engine.h"

void AForgeAnvil::ItemDectection(AActor * OverlappActor, bool entering)
{
	if (AForgePart* Part = Cast<AForgePart>(OverlappActor))
	{
		if (Part->PartType == EPartType::PT_UNFORMEDBLADE)
		{
			if (!entering)
			{
				Part->CurrentTouchingStation = nullptr;
				return;
			}
			Part->CurrentTouchingStation = this;
		}
		else
			ThrowAway(OverlappActor);
	}
	else
		ThrowAway(OverlappActor);
}

void AForgeAnvil::ProcessPartItem(AForgePart * Part)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Part: " + Part->GetName()));
	switch (Part->ResourceType)
	{
		case(EResource::R_REALBRONZE):
		{
			MakeResource(EBladeMat::BM_BRONZE);
			break;
		}
		case(EResource::R_STEEL):
		{
			MakeResource(EBladeMat::BM_IRON);
			break;
		}
		default:
		{
			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResource"), true);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("No valid Resource type, instead is " + EnumPtr->GetNameByValue((int64)Part->ResourceType).ToString()));
			break;
		}
	}
	Part->Destroy();
}

AForgePart * AForgeAnvil::MakeResource(EBladeMat type)
{
	switch (type)
	{
		case(EBladeMat::BM_IRON):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(IronBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
		case(EBladeMat::BM_BRONZE):
		{
			AForgePart * ResourceRef = GetWorld()->SpawnActor<AForgePart>(BronzeBladePart, ObjectPosition->GetComponentLocation(), ObjectPosition->GetComponentRotation());
			return ResourceRef;
		}
	}
	return nullptr;
}
