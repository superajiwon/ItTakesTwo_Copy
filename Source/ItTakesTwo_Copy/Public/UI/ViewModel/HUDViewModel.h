// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/ViewModelBase.h"
#include "Shared/ITTTypes.h"
#include "Shared/Struct/HUDPlayerInfo.h"
#include "HUDViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHUDChangedPlayerInfo,	const FHUDPlayerInfo&,	PlayerInfo);
class APlayerBase;
class UHPComponent;
UCLASS()
class ITTAKESTWO_COPY_API UHUDViewModel : public UViewModelBase
{
	GENERATED_BODY()
public:
	virtual void Initialize(TObjectPtr<APlayerController> PlayerController) override;
	virtual void Deinitialize() override;
	
	void BindPlayerHP();
	void UnbindPlayerHP();

public:
	void BindHostHP(APlayerBase* Player, UHPComponent* HPComp, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole);
	void BindClientHP(APlayerBase* Player, UHPComponent* HPComp, EPlayerSlot PlayerSlot, EPlayerRole PlayerRole);
	
private:
	UFUNCTION()
	void HostHpChanging(float HostHP, float HostMaxHP);
	UFUNCTION()
	void ClientHpChanging(float ClientHP, float ClientMaxHP);
	
public:
	UPROPERTY(BlueprintAssignable, Category = "HUD|HP")
	FHUDChangedPlayerInfo OnHUDHPChanged;
	
private:
	TWeakObjectPtr<APlayerBase> HostPlayer ;
	TWeakObjectPtr<UHPComponent> HostHPComponent ;
	
	TWeakObjectPtr<APlayerBase> ClientPlayer ;
	TWeakObjectPtr<UHPComponent> ClientHPComponent ;
	
	
private:
	FHUDPlayerInfo HostInfo;
	FHUDPlayerInfo ClientInfo;
	
};
