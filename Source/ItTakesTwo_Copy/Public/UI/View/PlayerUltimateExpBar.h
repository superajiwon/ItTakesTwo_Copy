
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUltimateExpBar.generated.h"

class UProgressBar;

UCLASS()
class ITTAKESTWO_COPY_API UPlayerUltimateExpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetIsUsing(float IsUsing);
	void SetProgress(float ExpProgress);
	void SetGaugeColor(FLinearColor Color);
	
	UFUNCTION()
	void UpdateGauge(float CurGauge, float MaxGauge);
	
public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_ExpGauge{};
		
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	float bIsUsing{0.0f}; // 0 -> 준비중, 1 -> 사용중
	float Progress{0.0f};
	float TargetProgress{0.0f};
	FLinearColor GaugeColor{FLinearColor::White};
	
	// FTimerHandle TimerHandle;
};
