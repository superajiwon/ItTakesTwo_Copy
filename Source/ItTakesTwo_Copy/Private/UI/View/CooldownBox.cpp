// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/View/CooldownBox.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCooldownBox::NativeConstruct()
{
	Super::NativeConstruct();

	if (IMG_CooldownBorder)
	{
		DynMaterial = IMG_CooldownBorder->GetDynamicMaterial();
	}
}

void UCooldownBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsOnCooldown) return;

	ElapsedTime += InDeltaTime;
	float Progress = FMath::Clamp(ElapsedTime / CooldownDuration, 0.f, 1.f);

	// 머티리얼 Progress 업데이트 (테두리가 차오름)
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(TEXT("Progress"), Progress);
	}

	// 남은 시간 텍스트 업데이트
	int32 RemainingSec = FMath::CeilToInt(CooldownDuration - ElapsedTime);
	if (TXT_Cooldown)
	{
		TXT_Cooldown->SetText(FText::AsNumber(RemainingSec));
	}

	// 쿨타임 완료
	if (Progress >= 1.f)
	{
		bIsOnCooldown = false;
		ElapsedTime = 0.f;

		if (DynMaterial)
		{
			DynMaterial->SetScalarParameterValue(TEXT("Progress"), 0.f);
		}
		if (TXT_Cooldown)
		{
			TXT_Cooldown->SetText(FText::FromString(TEXT("W")));
		}
	}
}

void UCooldownBox::StartCooldown()
{
	if (bIsOnCooldown) return;

	bIsOnCooldown = true;
	ElapsedTime = 0.f;

	if (TXT_Cooldown)
	{
		TXT_Cooldown->SetText(FText::AsNumber(1));
	}
}
