// Fill out your copyright notice in the Description page of Project Settings.

#include "ForgeAnvil.h"
#include "Items/Sword/BladePart.h"
#include "Engine/World.h"

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
