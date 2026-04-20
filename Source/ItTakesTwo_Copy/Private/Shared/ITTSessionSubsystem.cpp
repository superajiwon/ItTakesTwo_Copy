// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/ITTSessionSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

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
	LastSessionSettings->NumPublicConnections = NumPublicConnection;
	LastSessionSettings->bIsLANMatch = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bAllowJoinInProgress = true;
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
	LastSessionSearch->bIsLanQuery = true;
	// LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

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
