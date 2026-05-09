// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/ViewModelBase.h"
#include "HUDViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHUDHPChangedSignature, float, CurHP, float, MaxHP);

class APlayerBase;
class UHPComponent;

UCLASS()
class ITTAKESTWO_COPY_API UHUDViewModel : public UViewModelBase
{
	GENERATED_BODY()
public:
	virtual void Initialize(TObjectPtr<APlayerController> PlayerController) override;
	virtual void Deinitialize() override;
	void BindPlayerHP();
	void UnbindPlayerHP();
	void HandleHPChanged(float InCurHP, float InMaxHP);

public:
	UFUNCTION(BlueprintCallable, Category = "HUD|HP")
	float GetCurHP() const
	{
		return CurHP;
	}
	UFUNCTION(BlueprintCallable, Category = "HUD|HP")
	float GetMaxHP() const
	{
		return MaxHP;
	}
	UFUNCTION(BlueprintCallable, Category = "HUD|HP")
	float GetHPPercent() const
	{
		return MaxHP > 0.f ? CurHP / MaxHP : 0.f;
	}
	
public:
	UPROPERTY(BlueprintAssignable, Category = "HUD|HP")
	FHUDHPChangedSignature OnHUDHPChanged;
	
private:
	TWeakObjectPtr<APlayerBase> CachedPlayer;
	TWeakObjectPtr<UHPComponent> CachedHPComponent;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "HP", meta = (AllowPrivateAccess = "true"))
	float CurHP = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "HP", meta = (AllowPrivateAccess = "true"))
	float MaxHP = 0.f;
};
