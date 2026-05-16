
#include "UI/InGameHPBar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"

void UInGameHPBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UInGameHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	FProgressBarStyle Style = HPBar->GetWidgetStyle();
	// 2. WidgetBlueprintLibrary를 사용하여 브러시로부터 다이내믹 머티리얼을 추출합니다.
	// 이 함수는 브러시에 머티리얼이 없으면 생성해서 반환해줍니다!
	DynMat = UWidgetBlueprintLibrary::GetDynamicMaterial(Style.BackgroundImage);
	DynMat->SetVectorParameterValue(TEXT("FillColor_R"), FillColor);
	DynMat->SetVectorParameterValue(TEXT("HitColor_B"), HitColor);
	DynMat->SetVectorParameterValue(TEXT("BackGround_G"), BackGroundColor);
	
	// 머티리얼의 초기 채움 상태도 명시적으로 설정합니다 (안 보임 방지)
	DynMat->SetScalarParameterValue(TEXT("Fill"), Fill);
	DynMat->SetScalarParameterValue(TEXT("Hit"), Hit);
	
	HPBar->SetWidgetStyle(Style);
}

void UInGameHPBar::SetFill(float FillValue)
{
	Fill = FillValue;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Fill"), Fill);
	}
}

void UInGameHPBar::SetHit(float HitValue)
{
	Hit = HitValue;
	
	if (DynMat != nullptr)
	{
		DynMat->SetScalarParameterValue(TEXT("Hit"), Hit);
	}
}

void UInGameHPBar::SetPercent(float FillValue)
{
	SetFill(FillValue);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameHPBar::HitBarMovement, 0.01f, true);
}

void UInGameHPBar::HitBarMovement()
{
	double Val = FMath::FInterpTo(Hit, Fill, GetWorld()->GetDeltaSeconds(), 1.0f);
	SetHit(Val);
	
	if (Hit <= Fill || FMath::IsNearlyEqual(Hit, Fill, 0.001f))
	{
		SetHit(Fill);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UInGameHPBar::SetColors(FLinearColor FillLinearColor, FLinearColor HitLinearColor)
{
	FillColor = FillLinearColor;
	HitColor = HitLinearColor;
		
	if (DynMat != nullptr)
	{
		DynMat->SetVectorParameterValue(TEXT("FillColor_R"), FillColor);
		DynMat->SetVectorParameterValue(TEXT("HitColor_B"), HitColor);
	}
}

void UInGameHPBar::UpdateHP(float CurHp, float MaxHp)
{
	if (MaxHp <= 0.0f) return;
	
	float Percent = CurHp / MaxHp;
	UE_LOG(LogTemp, Warning, TEXT("CurHP(%f)/MaxHP(%f) : %f"), CurHp, MaxHp, Percent);
	if (Percent >= Fill)
	{
		SetFill(Percent);
		SetHit(Percent);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	else
	{
		SetPercent(Percent);
	}
}
