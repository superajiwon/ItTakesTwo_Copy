// Fill out your copyright notice in the Description page of Project Settings.


#include "UIMangerSubsystem.h"

#include "ItTakesTwo_Copy/UI/ViewModel/HUDViewModel/HUDViewModel.h"
#include "ItTakesTwo_Copy/UI/ViewModel/LobbyViewModel/LobbyViewModel.h"

void UUIMangerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);



}

void UUIMangerSubsystem::BeginLobbyUI()
{
	if (!LobbyViewModel)
	{
		LobbyViewModel = NewObject<ULobbyViewModel>(this);
		LobbyViewModel->Initialize(GetWorld()->GetFirstPlayerController());
	}
	
	
}

void UUIMangerSubsystem::BeginHUD()
{
	if (!HUDViewModel)
	{
		HUDViewModel = NewObject<UHUDViewModel>(this);
		HUDViewModel->Initialize(GetWorld()->GetFirstPlayerController());
	}
	
}

void UUIMangerSubsystem::EndLobbyUI()
{
	if (LobbyViewModel)
	{
		LobbyViewModel->Deinitialize();
		LobbyViewModel = nullptr;
	}
}

void UUIMangerSubsystem::EndHUD()
{
	if (HUDViewModel)
	{
		HUDViewModel->Deinitialize();
		HUDViewModel = nullptr;
	}


}
