
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Struct/HUDPlayerInfo.h"
#include "Shared/ITTTypes.h"
#include "MainGameUI.generated.h"

class UPlayerPortrait;
class UPlayerSkillList;
class UPlayerReviveBar;
class UHorizontalBox;

UCLASS()
class ITTAKESTWO_COPY_API UMainGameUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UFUNCTION()
	void OnHPChanged(const FHUDPlayerInfo& PlayerInfo);
	UFUNCTION()
	void OnSkillCooldownChanged(EPlayerRole PlayerRole, EActionType ActionType, bool bIsOnCooldown, float CooldownTime);
	UFUNCTION()
	void OnUltimateGaugeChanged(EPlayerRole PlayerRole, float CurGauge, float MaxGauge);
	UFUNCTION()
	void OnReviveTimeChanged(EPlayerRole PlayerRole, bool bIsDead, float ReviveTime);

public:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* Panel_May{};
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* Panel_Cody{};
	
	UPROPERTY(meta=(BindWidget))
	UPlayerPortrait* Portrait_May{};
	UPROPERTY(meta=(BindWidget))
	UPlayerPortrait* Portrait_Cody{};
	
	UPROPERTY(meta=(BindWidget))
	UPlayerSkillList* SkillList_May{};
	UPROPERTY(meta=(BindWidget))
	UPlayerSkillList* SkillList_Cody{};
	
	UPROPERTY(meta=(BindWidget))
	UPlayerReviveBar* ReviveBar_May{};
	UPROPERTY(meta=(BindWidget))
	UPlayerReviveBar* ReviveBar_Cody{};

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GettingSmallMay{};
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GettingSmallCody{};
};
