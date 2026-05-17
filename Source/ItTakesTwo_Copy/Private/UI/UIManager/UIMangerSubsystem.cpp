// Fill out your copyright notice in the Description page of Project Settings.


#include "ItTakesTwo_Copy/Public/UI/UIManager/UIMangerSubsystem.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/HUDViewModel.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/LobbyViewModel.h"
#include "UI/View/MainGameUI.h"

#include "Blueprint/UserWidget.h"

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

void UUIMangerSubsystem::BeginHUD(APlayerController* OwningPC)
{
	if (!OwningPC || !OwningPC->IsLocalController())
		return;

	if (!HUDViewModel)
	{
		HUDViewModel = NewObject<UHUDViewModel>(this);
		HUDViewModel->Initialize(OwningPC);
	}
	
	// 여기서 UserWidget 만들자!!
	if (!MainGameUIWidget)
	{
		FString WidgetPath = TEXT("/Game/UI/Blueprints/WBP_MainGameUI.WBP_MainGameUI_C"); 
		TSubclassOf<UMainGameUI> UIClass = LoadClass<UMainGameUI>(nullptr, *WidgetPath);
		
		if (UIClass)
		{
			MainGameUIWidget = CreateWidget<UMainGameUI>(OwningPC, UIClass);
			if (MainGameUIWidget)
			{
				MainGameUIWidget->AddToViewport();
			}
		}
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
	if (MainGameUIWidget)
	{
		MainGameUIWidget->RemoveFromParent();
		MainGameUIWidget = nullptr;
	}
	
	if (HUDViewModel)
	{
		HUDViewModel->Deinitialize();
		HUDViewModel = nullptr;
	}
}
