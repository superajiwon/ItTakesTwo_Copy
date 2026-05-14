
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerReviveBar.generated.h"

class UProgressBar;

UCLASS()
class ITTAKESTWO_COPY_API UPlayerReviveBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void SetIsSkillReady(bool IsSkillReady);
	void SetProgress(float ProgressValue);
	
	UFUNCTION()
	void UpdateSkillCoolTime(float CurCoolTime, float MaxCoolTime);
	
public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_Revive{};
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	float bIsReady{0.0f}; // 0 -> 준비중, 1 -> 사용가능
	float Progress{1.0f}; 
	
	FTimerHandle TimerHandle;
	
};
