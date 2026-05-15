
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerPortrait.generated.h"

class UProgressBar;

UCLASS()
class ITTAKESTWO_COPY_API UPlayerPortrait : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetProgress(float ProgressValue);
	void SetProgressColor(FLinearColor Color);
	void SetBackgroundColor(FLinearColor Color);
	
	UFUNCTION(BlueprintCallable)
	void UpdateHP(float CurHP, float MaxHP);
	
public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_HP{};
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	float Progress{1.0f}; 
	float TargetProgress{1.0f};
	FLinearColor ProgressBarColor{FLinearColor(0.646578f, 0.004345f, 0.f)};
	FLinearColor BackgroundColor{FLinearColor(0.039435f, 0.039435f, 0.039435f)};
	// FTimerHandle TimerHandle;
};
