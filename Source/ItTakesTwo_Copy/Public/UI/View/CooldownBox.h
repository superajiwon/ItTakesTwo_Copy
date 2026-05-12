// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CooldownBox.generated.h"

// 이 파일 위치: Source/ItTakesTwo_Copy/Public/UI/View/CooldownBox.h

class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;

UCLASS()
class ITTAKESTWO_COPY_API UCooldownBox : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 쿨타임 시작 (외부에서 호출)
	UFUNCTION(BlueprintCallable)
	void StartCooldown();

private:
	UPROPERTY(Meta = (BindWidget))
	UImage* IMG_CooldownBorder{};

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TXT_Cooldown{};

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial{};

	// 쿨타임 진행 중인지
	bool bIsOnCooldown = false;

	// 경과 시간
	float ElapsedTime = 0.f;

	// 쿨타임 총 시간 (1초)
	const float CooldownDuration = 1.f;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
