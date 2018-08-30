// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordItem.h"

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

bool ASwordItem::IsHandle(ESwordPart SwordPart)
{
	if (SwordPart == ESwordPart::PT_CURVEDHANDLE ||
		SwordPart == ESwordPart::PT_STRAIGHTHANDLE)
	{
		return true;
	}

	return false;
}

bool ASwordItem::IsBlade(ESwordPart SwordPart)
{
	if (SwordPart == ESwordPart::PT_BRONZEBROADSWORDBLADE ||
		SwordPart == ESwordPart::PT_BRONZEKRISBLADE ||
		SwordPart == ESwordPart::PT_STEELBROADSWORDBLADE ||
		SwordPart == ESwordPart::PT_STEELKRISBLADE)
	{
		return true;
	}
	return false;
}
