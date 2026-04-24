// Fill out your copyright notice in the Description page of Project Settings.


#include "ItTakesTwo_Copy/Public/UI/View/Lobby_View.h"

#include "Components/Button.h"
#include "Shared/MenuController.h"

void ULobby_View::NativeConstruct()
{
	Super::NativeConstruct();
	
	Btn_Create->OnClicked.AddDynamic(this,&ULobby_View::CreateServerHandler );
	Btn_Join->OnClicked.AddDynamic(this,&ULobby_View::JoinServerHandler );
}

void ULobby_View::CreateServerHandler()
{
	AMenuController* PC = Cast<AMenuController>(GetOwningPlayer());
	if (PC)
	{
		PC->HostStart();
	}
}

void ULobby_View::JoinServerHandler()
{
	AMenuController* PC = Cast<AMenuController>(GetOwningPlayer());
	if (PC)
	{
		PC->FindSession();
	}
	
	
}
