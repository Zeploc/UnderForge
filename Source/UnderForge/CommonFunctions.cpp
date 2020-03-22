// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonFunctions.h"

#include "Engine/World.h"

#include "UnderForge.h"


bool UCommonFunctions::IsWithEditor()
{
	return GIsEditor;
}

void UCommonFunctions::AddDebugMessage(FString _Message, bool _Error, bool _PrintToConsole)
{
	if (_PrintToConsole)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, _Error ? FColor::Red : FColor::Black, _Message);
		}
	}
	if (_Error)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *_Message);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *_Message);
	}
}

bool UCommonFunctions::IsHandle(ESwordPart SwordPart)
{
	if (SwordPart == ESwordPart::PT_BROADSWORDHANDLE ||
		SwordPart == ESwordPart::PT_KRISHANDLE)
	{
		return true;
	}

	return false;
}

bool UCommonFunctions::IsBlade(ESwordPart SwordPart)
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