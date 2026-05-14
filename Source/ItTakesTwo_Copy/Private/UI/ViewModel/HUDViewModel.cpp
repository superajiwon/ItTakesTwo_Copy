
#include "UI/ViewModel/HUDViewModel.h"

#include "Actors/Characters/Players/ITTPlayerState.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/HPComponent.h"
#include "Components/SkillComponent.h"
#include "Components/UltimateComponent.h"
#include "EngineUtils.h"

void UHUDViewModel::Initialize(TObjectPtr<APlayerController> PlayerController)
{
	Super::Initialize(PlayerController);

	// HUD 생성 시점에 복제된 플레이어 Pawn만 찾아 HP 델리게이트를 바인딩
	BindPlayerStats();
}

void UHUDViewModel::Deinitialize()
{
	UnbindPlayerStats();

	Super::Deinitialize();
}

void UHUDViewModel::BindPlayerStats()
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

		EPlayerSlot Slot = PlayerState->PlayerSlot;
		EPlayerRole Role = PlayerState->PlayerRole;
		
		if (Slot == EPlayerSlot::Host)
		{
			BindHostStats(Player, Slot, Role);
		}
		else if (Slot == EPlayerSlot::Client)
		{
			BindClientStats(Player, Slot, Role);
		}
	}
}

void UHUDViewModel::UnbindPlayerStats()
{
	if (HostHPComponent.IsValid())
	{
		HostHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HostHpChanging);
		HostHPComponent->OnDeath.RemoveDynamic(this, &UHUDViewModel::OnHostDeath);
		HostHPComponent->OnRevive.RemoveDynamic(this, &UHUDViewModel::OnHostRevive);
	}
	
	if (HostSkillComponent.IsValid())
	{
		HostSkillComponent->OnCooldownChange.RemoveDynamic(this, &UHUDViewModel::HostSkillCooldownChanging);
	}
	
	if (ClientHPComponent.IsValid())
	{
		ClientHPComponent->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::ClientHpChanging);
		ClientHPComponent->OnDeath.RemoveDynamic(this, &UHUDViewModel::OnClientDeath);
		ClientHPComponent->OnRevive.RemoveDynamic(this, &UHUDViewModel::OnClientRevive);
	}
	if (ClientUltimateComponent.IsValid())
	{
		ClientUltimateComponent->OnUltimateGaugeChanged.RemoveDynamic(this, &UHUDViewModel::ClientUltimateGaugeChanging);
	}

	HostPlayer.Reset();
	ClientPlayer.Reset();
	HostHPComponent.Reset();
	ClientHPComponent.Reset();
	HostSkillComponent.Reset();
	ClientSkillComponent.Reset();
	HostUltimateComponent.Reset();
	ClientUltimateComponent.Reset();
}

void UHUDViewModel::BindHostStats(APlayerBase* Player, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole)
{
	if (!Player) 
		return;
	
	HostPlayer = Player;
	HostInfo.Role = PlayerRole;
	HostInfo.Slot = PlayerSlot;
	
	// HP Binding
	if (UHPComponent* HPComp = Player->GetHPComponent())
	{
		HostHPComponent = HPComp;
		HostHpChanging(HPComp->GetCurHP(), HPComp->GetMaxHP());
		
		HPComp->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::HostHpChanging);
		HPComp->OnHPChanged.AddDynamic(this, &UHUDViewModel::HostHpChanging);
		
		HPComp->OnDeath.RemoveDynamic(this, &UHUDViewModel::OnHostDeath);
		HPComp->OnDeath.AddDynamic(this, &UHUDViewModel::OnHostDeath);
		
		HPComp->OnRevive.RemoveDynamic(this, &UHUDViewModel::OnHostRevive);
		HPComp->OnRevive.AddDynamic(this, &UHUDViewModel::OnHostRevive);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Host HPComp... "));
	
	// Skill Binding
	if (USkillComponent* SkillComp = Player->GetSkillComponent())
	{
		HostSkillComponent = SkillComp;
		
		SkillComp->OnCooldownChange.RemoveDynamic(this, &UHUDViewModel::HostSkillCooldownChanging);
		SkillComp->OnCooldownChange.AddDynamic(this, &UHUDViewModel::HostSkillCooldownChanging);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Host SkillComp... "));
	
	// Ultimate Binding
	if (UUltimateComponent* UltComp = Player->GetUltimateComponent())
	{
		HostUltimateComponent = UltComp;
		UltComp->OnUltimateGaugeChanged.RemoveDynamic(this, &UHUDViewModel::HostUltimateGaugeChanging);
		UltComp->OnUltimateGaugeChanged.AddDynamic(this, &UHUDViewModel::HostUltimateGaugeChanging);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Host UltimateComp... "));
}

void UHUDViewModel::BindClientStats(APlayerBase* Player, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole)
{
	if (!Player)
		return;
	
	ClientPlayer = Player;
	ClientInfo.Role = PlayerRole;
	ClientInfo.Slot = PlayerSlot;
	
	// HP Binding
	if (UHPComponent* HPComp = Player->GetHPComponent())
	{
		ClientHPComponent = HPComp;
		ClientHpChanging(HPComp->GetCurHP(), HPComp->GetMaxHP());
		
		HPComp->OnHPChanged.RemoveDynamic(this, &UHUDViewModel::ClientHpChanging);
		HPComp->OnHPChanged.AddDynamic(this, &UHUDViewModel::ClientHpChanging);
		
		HPComp->OnDeath.RemoveDynamic(this, &UHUDViewModel::OnClientDeath);
		HPComp->OnDeath.AddDynamic(this, &UHUDViewModel::OnClientDeath);
		
		HPComp->OnRevive.RemoveDynamic(this, &UHUDViewModel::OnClientRevive);
		HPComp->OnRevive.AddDynamic(this, &UHUDViewModel::OnClientRevive);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Client HPComp... "));
	
	// Skill Binding
	if (USkillComponent* SkillComp = Player->GetSkillComponent())
	{
		ClientSkillComponent = SkillComp;
		
		SkillComp->OnCooldownChange.RemoveDynamic(this, &UHUDViewModel::ClientSkillCooldownChanging);
		SkillComp->OnCooldownChange.AddDynamic(this, &UHUDViewModel::ClientSkillCooldownChanging);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Client SkillComp... "));
	
	// Ultimate Binding
	if (UUltimateComponent* UltComp = Player->GetUltimateComponent())
	{
		ClientUltimateComponent = UltComp;
		UltComp->OnUltimateGaugeChanged.RemoveDynamic(this, &UHUDViewModel::ClientUltimateGaugeChanging);
		UltComp->OnUltimateGaugeChanged.AddDynamic(this, &UHUDViewModel::ClientUltimateGaugeChanging);
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind Client UltimateComp... "));
}

void UHUDViewModel::HostHpChanging(float HostHP, float HostMaxHP)
{
	HostInfo.CurHP = HostHP;
    HostInfo.MaxHP = HostMaxHP;
	UE_LOG(LogTemp, Warning, TEXT("Host HP : %f   | MaxHP :  %f"),HostHP ,HostMaxHP);
	OnHUDHPChanged.Broadcast(HostInfo);
}

void UHUDViewModel::ClientHpChanging(float ClientHP, float ClientMaxHP)
{
	ClientInfo.CurHP = ClientHP;
    ClientInfo.MaxHP = ClientMaxHP;
	UE_LOG(LogTemp, Warning, TEXT("Client HP : %f   | MaxHP :  %f"),ClientHP ,ClientMaxHP);
	OnHUDHPChanged.Broadcast(ClientInfo);
}

void UHUDViewModel::HostSkillCooldownChanging(EActionType ActionType, bool bIsOnCooldown, float CooldownTime)
{
	OnHUDSkillCooldownChanged.Broadcast(HostInfo.Role, ActionType, bIsOnCooldown, CooldownTime);
}

void UHUDViewModel::ClientSkillCooldownChanging(EActionType ActionType, bool bIsOnCooldown, float CooldownTime)
{
	OnHUDSkillCooldownChanged.Broadcast(ClientInfo.Role, ActionType, bIsOnCooldown, CooldownTime);
}

void UHUDViewModel::HostUltimateGaugeChanging(float CurGauge, float MaxGauge)
{
	OnHUDUltimateGaugeChanged.Broadcast(HostInfo.Role, CurGauge, MaxGauge);
}

void UHUDViewModel::ClientUltimateGaugeChanging(float CurGauge, float MaxGauge)
{
	OnHUDUltimateGaugeChanged.Broadcast(ClientInfo.Role, CurGauge, MaxGauge);
}

void UHUDViewModel::HostReviveTimeChanging(bool bIsDead, float ReviveTime)
{
	OnHUDReviveTimeChanged.Broadcast(HostInfo.Role, bIsDead, ReviveTime);
}

void UHUDViewModel::ClientReviveTimeChanging(bool bIsDead, float ReviveTime)
{
	OnHUDReviveTimeChanged.Broadcast(ClientInfo.Role, bIsDead, ReviveTime);
}

void UHUDViewModel::OnHostDeath()
{
	HostReviveTimeChanging(true, 3.0f);
}

void UHUDViewModel::OnHostRevive()
{
	HostReviveTimeChanging(false, 3.0f);
}

void UHUDViewModel::OnClientDeath()
{
	ClientReviveTimeChanging(true, 3.0f);
}

void UHUDViewModel::OnClientRevive()
{
	ClientReviveTimeChanging(false, 3.0f);
}


