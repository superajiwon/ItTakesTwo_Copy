// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/HUDViewModel.h"

#include "EngineUtils.h"
#include "Actors/Characters/Players/ITTPlayerState.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/HPComponent.h"

void UHUDViewModel::Initialize(TObjectPtr<APlayerController> PlayerController)
{
	Super::Initialize(PlayerController);

	// HUD 생성 시점에 복제된 플레이어 Pawn만 찾아 HP 델리게이트를 바인딩합니다.
	BindPlayerHP();
}

void UHUDViewModel::Deinitialize()
{
	UnbindPlayerHP();

	Super::Deinitialize();
}

void UHUDViewModel::BindPlayerHP()
{
	if (!MyPlayerController.IsValid())
		return;
	
	UWorld* World = MyPlayerController->GetWorld();
	if (!World)
		return;
	
	for (TActorIterator<APlayerBase> It(World); It; ++It)
	{
		APlayerBase* Player = *It;
		if (!Player)
			continue;

		AITTPlayerState* PlayerState = Player->GetPlayerState<AITTPlayerState>();
		if (!PlayerState)
			continue;

		UHPComponent* HPComp = Player->GetHPComponent();
		if (!HPComp)
			continue;

		EPlayerSlot Slot = PlayerState->PlayerSlot;
		EPlayerRole Role = PlayerState->PlayerRole;
		
		if (Slot == EPlayerSlot::Host)
		{
			BindHostHP(Player, HPComp, Slot, Role);
		}
		else if (Slot == EPlayerSlot::Client)
		{
			BindClientHP(Player, HPComp, Slot, Role);
		}
	}
	
	
	
}

void UHUDViewModel::UnbindPlayerHP()
{
	if (HostHPComponent.IsValid())
	{
		HostHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HostHpChanging);
	}

	if (ClientHPComponent.IsValid())
	{
		ClientHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::ClientHpChanging);
	}

	HostPlayer.Reset();
	ClientPlayer.Reset();
	HostHPComponent.Reset();
	ClientHPComponent.Reset();
}

void UHUDViewModel::BindHostHP(APlayerBase* Player, UHPComponent* HPComp, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole)
{
	if (!Player || !HPComp)
		return;
	
	if (HostHPComponent.IsValid())
	{
		HostHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HostHpChanging);
	}
	
	HostPlayer = Player;
	HostHPComponent = HPComp;
	HostInfo.Role = PlayerRole;
	HostInfo.Slot = PlayerSlot;
	
	HostHpChanging(HPComp->GetCurHP(), HPComp->GetMaxHP());
	HPComp->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HostHpChanging);
	HPComp->OnHPChanged.AddDynamic(this, &UHUDViewModel::HostHpChanging);
	
	UE_LOG(LogTemp, Warning, TEXT("Bind Host HPComp "));
	UE_LOG(LogTemp, Warning, TEXT("Bind Host HPComp "));
	UE_LOG(LogTemp, Warning, TEXT("Bind Host HPComp "));
}

void UHUDViewModel::BindClientHP(APlayerBase* Player, UHPComponent* HPComp, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole)
{
	if (!Player || !HPComp)
		return;
	
	if (ClientHPComponent.IsValid())
	{
		ClientHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::ClientHpChanging);
	}
	
	ClientPlayer = Player;
	ClientHPComponent = HPComp;
	ClientInfo.Role = PlayerRole;
	ClientInfo.Slot = PlayerSlot;
	
	ClientHpChanging(HPComp->GetCurHP(), HPComp->GetMaxHP());
	HPComp->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::ClientHpChanging);
	HPComp->OnHPChanged.AddDynamic(this, &UHUDViewModel::ClientHpChanging);
	UE_LOG(LogTemp, Warning, TEXT("Bind Client HPComp "));
	UE_LOG(LogTemp, Warning, TEXT("Bind Client HPComp "));
	UE_LOG(LogTemp, Warning, TEXT("Bind Client HPComp "));
}

void UHUDViewModel::HostHpChanging(float HostHP, float HostMaxHP)
{
	HostInfo.CurHP = HostHP;
    HostInfo.MaxHP = HostMaxHP;
	UE_LOG(LogTemp, Warning, TEXT("Host HP : %f   | MaxHP :  %f"),HostHP ,HostMaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Host HP : %f   | MaxHP :  %f"),HostHP ,HostMaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Host HP : %f   | MaxHP :  %f"),HostHP ,HostMaxHP);
	OnHUDHPChanged.Broadcast(HostInfo);
}

void UHUDViewModel::ClientHpChanging(float ClientHP, float ClientMaxHP)
{
	ClientInfo.CurHP = ClientHP;
    ClientInfo.MaxHP = ClientMaxHP;
	UE_LOG(LogTemp, Warning, TEXT("Client HP : %f   | MaxHP :  %f"),ClientHP ,ClientMaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Client HP : %f   | MaxHP :  %f"),ClientHP ,ClientMaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Client HP : %f   | MaxHP :  %f"),ClientHP ,ClientMaxHP);
	OnHUDHPChanged.Broadcast(ClientInfo);
}


