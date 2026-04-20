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
	UGameplayStatics::OpenLevel(GetWorld(), FName("127.0.0.1")); // Level 이름 대신 IP 주소.
}

void UITTSessionSubsystem::FindSession()
{
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
