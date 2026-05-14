// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerReviveBar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"

void UPlayerReviveBar::NativeConstruct()
{
	Super::NativeConstruct();
		
	FProgressBarStyle Style = ProgressBar_Revive->GetWidgetStyle();
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetScalarParameterValue(TEXT("bIsReady"), bIsReady);
	DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	ProgressBar_Revive->SetWidgetStyle(Style);
}

void UPlayerReviveBar::SetIsSkillReady(bool IsSkillReady)
{
	bIsReady = IsSkillReady;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("bIsReady"), bIsReady);
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

void UPlayerReviveBar::UpdateSkillCoolTime(float CurCoolTime, float MaxCoolTime)
{
	double Val = FMath::FInterpTo(CurCoolTime, MaxCoolTime, GetWorld()->GetDeltaSeconds(), 5.0f);
	
	if (CurCoolTime <= MaxCoolTime || FMath::IsNearlyEqual(CurCoolTime, MaxCoolTime, 0.001f))
	{
		SetProgress(MaxCoolTime);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
