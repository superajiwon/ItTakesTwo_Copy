
#include "UI/View/PlayerSkillSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UPlayerSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	FProgressBarStyle Style = ProgressBar_Skill->GetWidgetStyle();
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetScalarParameterValue(TEXT("bIsReady"), bIsReady);
	DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	ProgressBar_Skill->SetWidgetStyle(Style);
}

void UPlayerSkillSlot::SetImages(UTexture2D* KeyTexture, UTexture2D* SkillTexture)
{
	Img_Key->SetBrushFromTexture(KeyTexture);
	Img_Skill->SetBrushFromTexture(SkillTexture);
}

void UPlayerSkillSlot::SetIsSkillReady(bool IsSkillReady)
{
	bIsReady = IsSkillReady;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("bIsReady"), bIsReady);
	}
}

void UPlayerSkillSlot::SetProgress(float ProgressValue)
{
	Progress = ProgressValue;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	}
}

void UPlayerSkillSlot::UpdateSkillCoolTime(float CurCoolTime, float MaxCoolTime)
{
	double Val = FMath::FInterpTo(CurCoolTime, MaxCoolTime, GetWorld()->GetDeltaSeconds(), 5.0f);
	
	if (CurCoolTime <= MaxCoolTime || FMath::IsNearlyEqual(CurCoolTime, MaxCoolTime, 0.001f))
	{
		SetProgress(MaxCoolTime);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
