#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"


#include "ITTSessionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FSteamFriendEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly)
	int32 FriendIndex = -1;

	UPROPERTY(BlueprintReadOnly)
	bool bIsOnline = false;
};

UCLASS()
class ITTAKESTWO_COPY_API UITTSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	
	// 여기서 세션 생성, 찾기, 참여를 하자
public:
	void CreateSession(int32 NumPublicConnection);
	void JoinSession();
	void FindSession();
	
	
public:// Steam 초대
	UFUNCTION(BlueprintCallable)
	void ReadSteamFriends();
	UFUNCTION(BlueprintCallable)
	TArray<FSteamFriendEntry> GetSteamFriendEntries() const;
	UFUNCTION(BlueprintCallable)
	void InviteFriendByIndex(int32 FriendIndex);
	void TrySendPendingInvite();
	
	
private:
	// 친구 목록 찾기
	void OnReadSteamFriendsComplete(int32 LocalUserNum,	bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
	void JoinSessionByResult(const FOnlineSessionSearchResult& SearchResult);

	void OnSessionUserInviteAccepted(
		bool bWasSuccessful,
		int32 LocalUserNum,
		FUniqueNetIdPtr UserId,
		const FOnlineSessionSearchResult& InviteResult
	);

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	bool bFindSession{false};
	
	UPROPERTY()
	FName GameSessionName{NAME_GameSession};
	
	UPROPERTY()
	FName ListenLevelName{FName("Lv_CharacterSelect")}; //Lv_ServerPlayerSelect
	
	// 스팀 
private:
	TArray<TSharedRef<FOnlineFriend>> CachedFriends;
	TArray<FSteamFriendEntry> CachedFriendEntries;

	FUniqueNetIdPtr PendingInviteFriendId;
	
	FOnSessionUserInviteAcceptedDelegate SessionInviteAcceptedDelegate;
	FDelegateHandle SessionInviteAcceptedDelegateHandle;
};
