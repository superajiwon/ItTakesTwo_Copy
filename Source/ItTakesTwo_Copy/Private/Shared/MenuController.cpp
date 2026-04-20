// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/MenuController.h"

#include "Shared/ITTSessionSubsystem.h"

void AMenuController::BeginPlay()
{
	Super::BeginPlay();



}

void AMenuController::HostStart()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->CreateSession(4);
}

void AMenuController::ClientJoin()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->JoinSession();
	
}

void AMenuController::FindSession()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->FindSession();
}
