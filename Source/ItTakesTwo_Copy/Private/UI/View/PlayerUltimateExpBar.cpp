
#include "UI/View/PlayerUltimateExpBar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"


void UPlayerUltimateExpBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	FProgressBarStyle Style = ProgressBar_ExpGauge->GetWidgetStyle();
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetScalarParameterValue(TEXT("IsUsing"), bIsUsing);
	DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	ProgressBar_ExpGauge->SetWidgetStyle(Style);
}

void UPlayerUltimateExpBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!FMath::IsNearlyEqual(Progress, TargetProgress, 0.001f))
	{
		float NewProgress = FMath::FInterpTo(Progress, TargetProgress, InDeltaTime, 5.0f);
		SetProgress(NewProgress);
	}
}

void UPlayerUltimateExpBar::SetIsUsing(float IsUsing)
{
	bIsUsing = IsUsing;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("IsUsing"), bIsUsing);
	}
}

void UPlayerUltimateExpBar::SetProgress(float ExpProgress)
{
	Progress = ExpProgress;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	}
}

void UPlayerUltimateExpBar::SetGaugeColor(FLinearColor Color)
{
	GaugeColor = Color;
		
	if (DynMat != nullptr)
	{
		DynMat->SetVectorParameterValue(TEXT("GaugeColor"), GaugeColor);
	}
}

void UPlayerUltimateExpBar::UpdateGauge(float CurGauge, float MaxGauge)
{
	if (MaxGauge > 0.0f)
	{
		TargetProgress = CurGauge / MaxGauge;
	}
	else
	{
		TargetProgress = 0.0f;
	}
}
