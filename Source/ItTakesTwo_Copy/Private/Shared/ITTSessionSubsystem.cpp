// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/ITTSessionSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlinePresenceInterface.h"

 
void UITTSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Initialize: SessionInterface is invalid."));
		return;
	}

	SessionInviteAcceptedDelegate =
		FOnSessionUserInviteAcceptedDelegate::CreateUObject(
			this,
			&UITTSessionSubsystem::OnSessionUserInviteAccepted
		);

	SessionInviteAcceptedDelegateHandle =
		SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(
			SessionInviteAcceptedDelegate
		);
}

void UITTSessionSubsystem::Deinitialize()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(SessionInviteAcceptedDelegateHandle);
	}

	Super::Deinitialize();
}

void UITTSessionSubsystem::CreateSession(int32 NumPublicConnection)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSession failed: SessionInterface is invalid."));
		return;
	}

	if (SessionInterface->GetNamedSession(GameSessionName) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession aborted: Session already exists."));
		return;
	}

	CreateSessionCompleteDelegate =
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UITTSessionSubsystem::OnCreateSessionComplete);

	CreateSessionCompleteDelegateHandle =
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	 LastSessionSettings = MakeShared<FOnlineSessionSettings>();
	
	// Local에서 구현했을 때
	// LastSessionSettings->NumPublicConnections = NumPublicConnection;
	// LastSessionSettings->bIsLANMatch = true;
	// LastSessionSettings->bShouldAdvertise = true;
	// LastSessionSettings->bAllowJoinInProgress = true;
	// LastSessionSettings->bAllowJoinViaPresence = true;
	// LastSessionSettings->bUsesPresence = true;
	// LastSessionSettings->bUseLobbiesIfAvailable = true;
	
	// 스팀으로 초대하는 구현
	LastSessionSettings->NumPublicConnections = NumPublicConnection;
	LastSessionSettings->bIsLANMatch = false;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSession failed: LocalPlayer is null."));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		return;
	}

	const bool bCreateStarted = SessionInterface->CreateSession(
		*LocalPlayer->GetPreferredUniqueNetId(),
		GameSessionName,
		*LastSessionSettings
	);

	if (!bCreateStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSession failed: CreateSession call returned false."));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
}

void UITTSessionSubsystem::JoinSession()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: SessionInterface is invalid."));
		return;
	}

	if (!LastSessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: LastSessionSearch is invalid."));
		return;
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinSession failed: No search results found."));
		return;
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: LocalPlayer is null."));
		return;
	}

	JoinSessionCompleteDelegate =
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &UITTSessionSubsystem::OnJoinSessionComplete);

	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const FOnlineSessionSearchResult& SearchResult = LastSessionSearch->SearchResults[0];

	const bool bJoinStarted = SessionInterface->JoinSession(
		*LocalPlayer->GetPreferredUniqueNetId(),
		GameSessionName,
		SearchResult
	);

	if (!bJoinStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: JoinSession call returned false."));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
}

void UITTSessionSubsystem::FindSession()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("FindSession failed: SessionInterface is invalid."));
		return;
	}

	FindSessionsCompleteDelegate =
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &UITTSessionSubsystem::OnFindSessionsComplete);

	FindSessionsCompleteDelegateHandle =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShared<FOnlineSessionSearch>();
	LastSessionSearch->MaxSearchResults = 10;
	LastSessionSearch->bIsLanQuery = false;
	LastSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("FindSession failed: LocalPlayer is null."));
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		return;
	}

	const bool bFindStarted =
		SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef());

	if (!bFindStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("FindSession failed: FindSessions call returned false."));
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
}

void UITTSessionSubsystem::TrySendPendingInvite()
{
	if (!PendingInviteFriendId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySendPendingInvite: PendingInviteFriendId is invalid."));
		return;
	}

	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("TrySendPendingInvite failed: SessionInterface is invalid."));
		return;
	}

	if (SessionInterface->GetNamedSession(GameSessionName) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TrySendPendingInvite failed: Session does not exist."));
		return;
	}

	const bool bSent = SessionInterface->SendSessionInviteToFriend(
		0,
		GameSessionName,
		*PendingInviteFriendId
	);

	UE_LOG(LogTemp, Warning, TEXT("Steam Invite Sent: %d"), bSent);

	PendingInviteFriendId.Reset();
}

void UITTSessionSubsystem::ReadSteamFriends()
{
	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface(GetWorld());
	if (!FriendsInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ReadSteamFriends failed: FriendsInterface is invalid."));
		return;
	}

	const bool bReadStarted = FriendsInterface->ReadFriendsList(
		0,
		EFriendsLists::ToString(EFriendsLists::Default),
		FOnReadFriendsListComplete::CreateUObject(
			this,
			&UITTSessionSubsystem::OnReadSteamFriendsComplete
		)
	);

	if (!bReadStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("ReadSteamFriends failed: ReadFriendsList returned false."));
	}
}

TArray<FSteamFriendEntry> UITTSessionSubsystem::GetSteamFriendEntries() const
{
	return CachedFriendEntries;
}

void UITTSessionSubsystem::InviteFriendByIndex(int32 FriendIndex)
{
	
	if (!CachedFriends.IsValidIndex(FriendIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("초대 인덱스 실패"));
		return;
	}
	// 선택한 친구 NetId 저장
	PendingInviteFriendId = CachedFriends[FriendIndex]->GetUserId();
	
	// 내가 Host가 되기 위해 세션 생성
	CreateSession(2);
}

void UITTSessionSubsystem::OnReadSteamFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
	const FString& ErrorStr)
{
	CachedFriends.Empty();
	CachedFriendEntries.Empty();

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("ReadSteamFriends failed: %s"), *ErrorStr);
		return;
	}

	IOnlineFriendsPtr FriendsInterface = Online::GetFriendsInterface(GetWorld());
	if (!FriendsInterface.IsValid())
		return;

	if (!FriendsInterface->GetFriendsList(LocalUserNum, ListName, CachedFriends))
	{
		UE_LOG(LogTemp, Error, TEXT("GetFriendsList failed."));
		return;
	}

	for (int32 i = 0; i < CachedFriends.Num(); ++i)
	{
		const TSharedRef<FOnlineFriend>& Friend = CachedFriends[i];

		FSteamFriendEntry Entry;
		Entry.DisplayName = Friend->GetDisplayName();
		Entry.FriendIndex = i;
		Entry.bIsOnline = Friend->GetPresence().bIsOnline;

		CachedFriendEntries.Add(Entry);
	}

	UE_LOG(LogTemp, Warning, TEXT("Steam Friends Loaded: %d"), CachedFriendEntries.Num());
}

void UITTSessionSubsystem::JoinSessionByResult(const FOnlineSessionSearchResult& SearchResult)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSessionByResult failed: SessionInterface is invalid."));
		return;
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSessionByResult failed: LocalPlayer is null."));
		return;
	}

	JoinSessionCompleteDelegate =
		FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&UITTSessionSubsystem::OnJoinSessionComplete
		);

	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
			JoinSessionCompleteDelegate
		);

	const bool bJoinStarted = SessionInterface->JoinSession(
		*LocalPlayer->GetPreferredUniqueNetId(),
		GameSessionName,
		SearchResult
	);

	if (!bJoinStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSessionByResult failed: JoinSession returned false."));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
}

void UITTSessionSubsystem::OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, FUniqueNetIdPtr UserId,
	const FOnlineSessionSearchResult& InviteResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSessionUserInviteAccepted: Success=%d"), bWasSuccessful);

	if (!bWasSuccessful)
		return;

	if (!InviteResult.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OnSessionUserInviteAccepted failed: InviteResult is invalid."));
		return;
	}

	JoinSessionByResult(InviteResult);
}

void UITTSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("OnCreateSessionComplete failed: %s"), *SessionName.ToString());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Session created successfully. Opening listen level."));

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenLevel failed: World is null."));
		return;
	}

	UGameplayStatics::OpenLevel(World, ListenLevelName, true, TEXT("listen"));
}

void UITTSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	bFindSession = false;

	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("OnFindSessionsComplete failed."));
		return;
	}

	if (!LastSessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OnFindSessionsComplete failed: LastSessionSearch is invalid."));
		return;
	}

	const int32 ResultCount = LastSessionSearch->SearchResults.Num();
	UE_LOG(LogTemp, Log, TEXT("FindSession success: %d session(s) found."), ResultCount);

	for (int32 Index = 0; Index < ResultCount; ++Index)
	{
		const FOnlineSessionSearchResult& Result = LastSessionSearch->SearchResults[Index];

		const FString SessionId = Result.GetSessionIdStr();
		const FString OwnerName = Result.Session.OwningUserName;

		UE_LOG(LogTemp, Log, TEXT("Result[%d] SessionId: %s / Owner: %s"),
			Index,
			*SessionId,
			*OwnerName);
	}

	if (ResultCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found. Auto join aborted."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Session found. Auto joining first result."));
	JoinSession();
}

void UITTSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("OnJoinSessionComplete failed: %d"), static_cast<int32>(Result));
		return;
	}

	FString ConnectString;
	if (!SessionInterface.IsValid() || !SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		UE_LOG(LogTemp, Error, TEXT("OnJoinSessionComplete failed: Could not resolve connect string."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("JoinSession succeeded. ConnectString: %s"), *ConnectString);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("OnJoinSessionComplete failed: PlayerController is null."));
		return;
	}

	PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
}
