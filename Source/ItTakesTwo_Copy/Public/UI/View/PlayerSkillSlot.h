
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSkillSlot.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class ITTAKESTWO_COPY_API UPlayerSkillSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void SetImages(UTexture2D* KeyTexture, UTexture2D* SkillTexture);
	
	void SetIsSkillReady(bool IsSkillReady);
	void SetProgress(float ProgressValue);
	
	UFUNCTION()
	void UpdateSkillCoolTime(float CurCoolTime, float MaxCoolTime);
	
public:
	UPROPERTY(meta=(BindWidget))
	UImage* Img_Key{};
	
	UPROPERTY(meta=(BindWidget))
	UImage* Img_Skill{};
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_Skill{};
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	float bIsReady{0.0f}; // 0 -> 준비중, 1 -> 사용가능
	float Progress{1.0f}; 
	
	FTimerHandle TimerHandle;
};
