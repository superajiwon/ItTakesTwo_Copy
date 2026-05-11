
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHPBar.generated.h"

class UProgressBar;

struct FLinearColor;

UCLASS()
class ITTAKESTWO_COPY_API UInGameHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	void SetFill(float FillValue);
	void SetHit(float HitValue);
	void SetPercent(float FillValue);
	
	void HitBarMovement();
	
	UFUNCTION()
	void SetColors(FLinearColor FillLinearColor, FLinearColor HitLinearColor = FLinearColor::White);
	UFUNCTION()
	void UpdateHP(float CurHp, float MaxHp);
	
public:
	// 총알 위젯이 추가될 패널
	UPROPERTY(BlueprintReadWrite, Category="UI", meta=(BindWidget))
	UProgressBar* HPBar;
	
	float Fill = 1.0f;
	float Hit = 1.0f;
	
	FLinearColor FillColor;
	FLinearColor HitColor;
	FLinearColor BackGroundColor;
	
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
};
