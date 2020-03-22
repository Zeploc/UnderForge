// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "AdvancedFriendsGameInstance.h"
#include "Online.h"
#include "Engine/NetDriver.h"
#include "NWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFORGE_API UNWGameInstance : public UGameInstance //UAdvancedFriendsGameInstance// 
{
	GENERATED_BODY()

public:
	//UNWGameInstance();
	UNWGameInstance(const FObjectInitializer& ObjectInitializer);

	//#############--Host_Session--#############
private:
	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
		
	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
		
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/**
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

public:
	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);


	//#############--Find_Session--#############
private:
	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	
			
	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

public:
	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


	//#############--Join_Session--#############
private:
	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
		
	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
		void ClientTravelController(class APlayerController* Controller);

public:
	/**
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinGameSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//#############--Kick_Client--#############

	public:
		/**
		*	Joins a session via a search result
		*
		*	@param SessionName name of session
		*	@param SearchResult Session to join
		*
		*	@return bool true if successful, false otherwise
		*/
		bool KickClient(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//#############--Destroy_Session--#############
private:
	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	
	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
	/**
	*	Destroy current session
	*
	*	@param Session name to destroy
	*/
	void DestroySession(FName SessionName);


	void HandleNetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName CurrentSessionName;

	UFUNCTION(BlueprintImplementableEvent)
		void ServersComplete();

	UPROPERTY(BlueprintReadOnly)
		TMap<FName, int> SearchResults;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int HostNumberPlayers = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bJoinInProgress = false;

	class ANetworkController* PassThroughController = nullptr;
};
