// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"



#include "NetworkFunctionLibrary.generated.h"

UENUM(Blueprintable)
enum class ESteamAvatarSize : uint8
{
	SteamAvatar_Small = 1,
	SteamAvatar_Medium = 2,
	SteamAvatar_Large = 3
};

//General Advanced Sessions Log
DECLARE_LOG_CATEGORY_EXTERN(NetworkSessionLog, Log, All);
/**
 * 
 */
UCLASS()
class UNDERFORGE_API UNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//UFUNCTION(BlueprintPure)
	static void IsAFriend(APlayerController* PlayerController, TSharedPtr<const FUniqueNetId> UniqueNetId, bool& IsFriend);

	//static class UTexture2D* GetSteamFriendAvatar(TSharedPtr<const FUniqueNetId> UniqueNetId, ESteamAvatarSize AvatarSize);

};
