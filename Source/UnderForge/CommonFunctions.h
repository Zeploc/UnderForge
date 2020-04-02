// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utlities.h"
#include "CommonFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API UCommonFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
				
	UFUNCTION(BlueprintPure)
		static bool IsWithEditor();


	UFUNCTION(BlueprintCallable)
		static void AddDebugMessage(FString _Message, bool _Error = false, bool _PrintToConsole = true);

	UFUNCTION(BlueprintPure, Category = "Underforge Singleton")
		static class UUnderForgeSingleton* GetGameSingleton(bool& IsValid);

};
