// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerReviveBar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"

void UPlayerReviveBar::NativeConstruct()
{
	Super::NativeConstruct();
		
	FProgressBarStyle Style = ProgressBar_Revive->GetWidgetStyle();
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	ProgressBar_Revive->SetWidgetStyle(Style);
}

void UPlayerReviveBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (CurReviveTime > 0.0f)
	{
		CurReviveTime -= InDeltaTime;
		if (CurReviveTime <= 0.0f)
		{
			CurReviveTime = 0.0f;
			SetProgress(1.0f);
		}
		else
		{
			SetProgress(1.0f - (CurReviveTime / MaxReviveTime));
		}
	}
}

void UPlayerReviveBar::SetProgress(float ProgressValue)
{
	Progress = ProgressValue;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	}
}

void UPlayerReviveBar::StartRevive(bool bIsDead, float ReviveTime)
{
	if (bIsDead && ReviveTime > 0.0f)
	{
		CurReviveTime = ReviveTime;
		MaxReviveTime = ReviveTime;
		SetProgress(0.0f);
	}
	else
	{
		CurReviveTime = 0.0f;
		SetProgress(1.0f);
	}
}

