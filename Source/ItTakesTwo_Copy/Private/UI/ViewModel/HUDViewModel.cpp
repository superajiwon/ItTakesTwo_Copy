// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/HUDViewModel.h"

#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/HPComponent.h"

void UHUDViewModel::Initialize(TObjectPtr<APlayerController> PlayerController)
{
	Super::Initialize(PlayerController);
	
	BindPlayerHP();
}

void UHUDViewModel::Deinitialize()
{
	UnbindPlayerHP();

	CachedPlayer.Reset();
	Super::Deinitialize();
}

void UHUDViewModel::BindPlayerHP()
{
	if (!MyPlayerController.IsValid())
		return;

	APlayerBase* Player = Cast<APlayerBase>(MyPlayerController->GetPawn());
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUDViewModel] Player pawn is null"));
		return;
	}

	UHPComponent* HPComp = Player->GetHPComponent();
	if (!HPComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUDViewModel] HPComponent is null"));
		return;
	}

	CachedPlayer = Player;
	CachedHPComponent = HPComp;

	HPComp->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HandleHPChanged);
	HPComp->OnHPChanged.AddDynamic(this, &UHUDViewModel::HandleHPChanged);

	HandleHPChanged(HPComp->GetCurHP(), HPComp->GetMaxHP());
}

void UHUDViewModel::UnbindPlayerHP()
{
	if (CachedHPComponent.IsValid())
	{
		CachedHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HandleHPChanged);
	}

	CachedHPComponent.Reset();
}

void UHUDViewModel::HandleHPChanged(float InCurHP, float InMaxHP)
{
	CurHP = InCurHP;
	MaxHP = InMaxHP;

	OnHUDHPChanged.Broadcast(CurHP, MaxHP);
}