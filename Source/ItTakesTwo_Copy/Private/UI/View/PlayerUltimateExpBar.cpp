
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
	double Val = FMath::FInterpTo(CurGauge, MaxGauge, GetWorld()->GetDeltaSeconds(), 5.0f);
	
	if (CurGauge <= MaxGauge || FMath::IsNearlyEqual(CurGauge, MaxGauge, 0.001f))
	{
		SetProgress(MaxGauge);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
