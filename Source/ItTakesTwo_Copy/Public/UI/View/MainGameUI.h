
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameUI.generated.h"

class UPlayerSkillList;
class UPlayerReviveBar;
class UHorizontalBox;

UCLASS()
class ITTAKESTWO_COPY_API UMainGameUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* Panel_May{};
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* Panel_Cody{};
	
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
