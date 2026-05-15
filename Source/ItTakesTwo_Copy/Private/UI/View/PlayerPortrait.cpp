// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerPortrait.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"

void UPlayerPortrait::NativeConstruct()
{
	Super::NativeConstruct();
	
	FProgressBarStyle Style = ProgressBar_HP->GetWidgetStyle();
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	DynMat->SetVectorParameterValue(TEXT("ProgressBarColor"), ProgressBarColor);	
	DynMat->SetVectorParameterValue(TEXT("BackgroundColor"), BackgroundColor);	
	ProgressBar_HP->SetWidgetStyle(Style);
}

void UPlayerPortrait::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!FMath::IsNearlyEqual(Progress, TargetProgress, 0.001f))
	{
		float NewProgress = FMath::FInterpTo(Progress, TargetProgress, InDeltaTime, 5.0f);
		SetProgress(NewProgress);
	}
}

void UPlayerPortrait::SetProgress(float ProgressValue)
{
	Progress = ProgressValue;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	}
}

void UPlayerPortrait::SetProgressColor(FLinearColor Color)
{
	ProgressBarColor = Color;
		
	if (DynMat != nullptr)
	{	
		DynMat->SetVectorParameterValue(TEXT("ProgressBarColor"), ProgressBarColor);	
	}
}

void UPlayerPortrait::SetBackgroundColor(FLinearColor Color)
{
	BackgroundColor = Color;
		
	if (DynMat != nullptr)
	{	
		DynMat->SetVectorParameterValue(TEXT("BackgroundColor"), BackgroundColor);	
	}
}

void UPlayerPortrait::UpdateHP(float CurHP, float MaxHP)
{
	if (MaxHP > 0.0f)
	{
		TargetProgress = CurHP / MaxHP;
	}
	else
	{
		TargetProgress = 0.0f;
	}
}
