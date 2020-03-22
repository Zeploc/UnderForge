// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkFunctionLibrary.h"


//#include "OnlineSubsystem.h"
#include "Online.h"
#include "Interfaces/OnlineFriendsInterface.h"
//#include "Steam/steam_api.h"
//
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlinePresenceInterface.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Texture2D.h"


//General Log
DEFINE_LOG_CATEGORY(NetworkSessionLog);

void UNetworkFunctionLibrary::IsAFriend(APlayerController* PlayerController, TSharedPtr<const FUniqueNetId> UniqueNetId, bool& IsFriend)
{
	if (!PlayerController)
	{
		UE_LOG(NetworkSessionLog, Warning, TEXT("IsAFriend Had a bad Player Controller!"));
		return;
	}

	if (!UniqueNetId.IsValid())
	{
		UE_LOG(NetworkSessionLog, Warning, TEXT("IsAFriend Had a bad UniqueNetId!"));
		return;
	}

	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface();

	if (!FriendsInterface.IsValid())
	{
		UE_LOG(NetworkSessionLog, Warning, TEXT("IsAFriend Failed to get friends interface!"));
		return;
	}

	ULocalPlayer* Player = Cast<ULocalPlayer>(PlayerController->Player);

	if (!Player)
	{
		UE_LOG(NetworkSessionLog, Warning, TEXT("IsAFriend Failed to get LocalPlayer!"));
		return;
	}

	IsFriend = FriendsInterface->IsFriend(Player->GetControllerId(), *UniqueNetId, EFriendsLists::ToString(EFriendsLists::Default));
}


//UTexture2D* UNetworkFunctionLibrary::GetSteamFriendAvatar(TSharedPtr<const FUniqueNetId> UniqueNetId, ESteamAvatarSize AvatarSize)
//{
//#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX
//	if (!UniqueNetId.IsValid() || !UniqueNetId->IsValid() || UniqueNetId->GetType() != STEAM_SUBSYSTEM)
//	{
//		UE_LOG(NetworkSessionLog, Warning, TEXT("GetSteamFriendAvatar Had a bad UniqueNetId!"));
//		return nullptr;
//	}
//
//	uint32 Width = 0;
//	uint32 Height = 0;
//
//	if (SteamAPI_Init())
//	{
//		//Getting the PictureID from the SteamAPI and getting the Size with the ID
//		//virtual bool RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly ) = 0;
//
//
//		uint64 id = *((uint64*)UniqueNetId->GetBytes());
//		int Picture = 0;
//
//		switch (AvatarSize)
//		{
//		case ESteamAvatarSize::SteamAvatar_Small: Picture = SteamFriends()->GetSmallFriendAvatar(id); break;
//		case ESteamAvatarSize::SteamAvatar_Medium: Picture = SteamFriends()->GetMediumFriendAvatar(id); break;
//		case ESteamAvatarSize::SteamAvatar_Large: Picture = SteamFriends()->GetLargeFriendAvatar(id); break;
//		default: break;
//		}
//
//		if (Picture == -1)
//		{
//			return NULL;
//		}
//
//		SteamUtils()->GetImageSize(Picture, &Width, &Height);
//
//		// STOLEN FROM ANSWERHUB :p, then fixed because answerhub wasn't releasing the memory O.o
//		// Also fixed image pixel format and switched to a memcpy instead of manual iteration.
//		// At some point I should probably reply to that answerhub post with these fixes to prevent people killing their games.....
//
//		if (Width > 0 && Height > 0)
//		{
//			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
//			uint8* oAvatarRGBA = new uint8[Width * Height * 4];
//
//
//			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
//			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));
//
//
//			// Removed as I changed the image bit code to be RGB, I think the original author was unaware that there were different pixel formats
//			/*
//			//Swap R and B channels because for some reason the games whack
//			for (uint32 i = 0; i < (Width * Height * 4); i += 4)
//			{
//				uint8 Temp = oAvatarRGBA[i + 0];
//				oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
//				oAvatarRGBA[i + 2] = Temp;
//			}*/
//
//			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
//			// Switched to a Memcpy instead of byte by byte transer
//			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
//			Avatar->PlatformData->Mips[0].BulkData.Unlock();
//
//			// Original implementation was missing this!!
//			// the hell man......
//			delete[] oAvatarRGBA;
//
//			//Setting some Parameters for the Texture and finally returning it
//			Avatar->PlatformData->NumSlices = 1;
//			Avatar->NeverStream = true;
//			//Avatar->CompressionSettings = TC_EditorIcon;
//
//			Avatar->UpdateResource();
//
//			return Avatar;
//		}
//		else
//		{
//			UE_LOG(NetworkSessionLog, Warning, TEXT("Bad Height / Width with steam avatar!"));
//		}
//
//		return nullptr;
//	}
//#endif
//
//	UE_LOG(NetworkSessionLog, Warning, TEXT("STEAM Couldn't be verified as initialized"));
//	return nullptr;
//}