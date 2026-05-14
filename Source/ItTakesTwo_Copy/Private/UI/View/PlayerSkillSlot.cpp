
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

void UPlayerSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (CurCooldown > 0.0f)
	{
		CurCooldown -= InDeltaTime;
		if (CurCooldown <= 0.0f)
		{
			CurCooldown = 0.0f;
			SetProgress(1.f);
			SetIsSkillReady(false);
		}
		else
		{
			SetProgress(1.0f - (CurCooldown / MaxCooldown)); //? 왜
		}
	}
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

void UPlayerSkillSlot::UpdateSkillCoolTime(bool bIsOnCooldown, float CooldownTime)
{
	if (bIsOnCooldown && CooldownTime > 0.0f)
	{
		CurCooldown = CooldownTime;
		MaxCooldown = CooldownTime;
		SetProgress(0.0f);
		SetIsSkillReady(false);
	}
	else
	{
		CurCooldown = 0.0f;
		SetProgress(1.0f);
		SetIsSkillReady(true);
	}
}
