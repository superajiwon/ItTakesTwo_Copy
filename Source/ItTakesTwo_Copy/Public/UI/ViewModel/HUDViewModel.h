// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/ViewModelBase.h"
#include "Shared/ITTTypes.h"
#include "Shared/Struct/HUDPlayerInfo.h"
#include "HUDViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHUDChangedPlayerInfo,	const FHUDPlayerInfo&,	PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHUDSkillCooldownChanged, EPlayerRole, PlayerRole, EActionType, ActionType, bool, bIsOnCooldown, float, CooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHUDUltimateGaugeChanged, EPlayerRole, PlayerRole, float, CurGauge, float, MaxGauge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHUDReviveTimeChanged, EPlayerRole, PlayerRole, bool, bIsDead, float, ReviveTime);

class APlayerBase;
class UHPComponent;
class USkillComponent;
class UUltimateComponent;

UCLASS()
class ITTAKESTWO_COPY_API UHUDViewModel : public UViewModelBase
{
	GENERATED_BODY()
public:
	virtual void Initialize(TObjectPtr<APlayerController> PlayerController) override;
	virtual void Deinitialize() override;
	
	void BindPlayerStats();
	void UnbindPlayerStats();

public:
	void BindHostStats(APlayerBase* Player, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole);
	void BindClientStats(APlayerBase* Player, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole);
	
private:
	UFUNCTION() 
	void HostHpChanging(float HostHP, float HostMaxHP);
	UFUNCTION()
	void ClientHpChanging(float ClientHP, float ClientMaxHP);
	
	UFUNCTION()
	void HostSkillCooldownChanging(EActionType ActionType, bool bIsOnCooldown, float CooldownTime);
	UFUNCTION()
	void ClientSkillCooldownChanging(EActionType ActionType, bool bIsOnCooldown, float CooldownTime);
	
	UFUNCTION()
	void HostUltimateGaugeChanging(float CurGauge, float MaxGauge);
	UFUNCTION()
	void ClientUltimateGaugeChanging(float CurGauge, float MaxGauge);
	
	UFUNCTION()
	void HostReviveTimeChanging(bool bIsDead, float ReviveTime);
	UFUNCTION()
	void ClientReviveTimeChanging(bool bIsDead, float ReviveTime);
	
	UFUNCTION()
	void OnHostDeath();
	UFUNCTION()
	void OnHostRevive();
	
	UFUNCTION()
	void OnClientDeath();
	UFUNCTION()
	void OnClientRevive();
	
public:
	UPROPERTY(BlueprintAssignable, Category = "HUD|HP")
	FHUDChangedPlayerInfo OnHUDHPChanged;
	UPROPERTY(BlueprintAssignable, Category = "HUD|Skill")
	FHUDSkillCooldownChanged OnHUDSkillCooldownChanged;
	UPROPERTY(BlueprintAssignable, Category = "HUD|Ultimate")
	FHUDUltimateGaugeChanged OnHUDUltimateGaugeChanged;
	UPROPERTY(BlueprintAssignable, Category = "HUD|Revive")
	FHUDReviveTimeChanged OnHUDReviveTimeChanged;
	
private:
	TWeakObjectPtr<APlayerBase> HostPlayer;
	TWeakObjectPtr<UHPComponent> HostHPComponent;
	TWeakObjectPtr<USkillComponent> HostSkillComponent;
	TWeakObjectPtr<UUltimateComponent> HostUltimateComponent;
	
	TWeakObjectPtr<APlayerBase> ClientPlayer;
	TWeakObjectPtr<UHPComponent> ClientHPComponent;
	TWeakObjectPtr<USkillComponent> ClientSkillComponent;
	TWeakObjectPtr<UUltimateComponent> ClientUltimateComponent;
	
private:
	FHUDPlayerInfo HostInfo;
	FHUDPlayerInfo ClientInfo;
	
};
