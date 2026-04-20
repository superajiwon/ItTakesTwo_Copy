// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/LobbyViewModel.h"

void ULobbyViewModel::Initialize(TObjectPtr<APlayerController> PlayerController)
{
	Super::Initialize(PlayerController);
	// 델리게이트  바인드
// 	MyPlayerController->GetWorld();
}

void ULobbyViewModel::Deinitialize()
{
	// 델리게이트 릴리즈
	Super::Deinitialize();
}
