// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Main_View.generated.h"

// 이 파일 위치: Source/ItTakesTwo_Copy/Public/UI/View/Main_View.h

class UCooldownBox;

UCLASS()
class ITTAKESTWO_COPY_API UMain_View : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// May 스킬 쿨타임 시작
	UFUNCTION(BlueprintCallable)
	void StartMaySkill1Cooldown();

	UFUNCTION(BlueprintCallable)
	void StartMaySkill2Cooldown();

	UFUNCTION(BlueprintCallable)
	void StartMaySkill3Cooldown();

	// Cody 스킬 쿨타임 시작
	UFUNCTION(BlueprintCallable)
	void StartCodySkill1Cooldown();

	UFUNCTION(BlueprintCallable)
	void StartCodySkill2Cooldown();

	UFUNCTION(BlueprintCallable)
	void StartCodySkill3Cooldown();

private:
	// May 스킬 쿨다운 박스 (순서대로 Skill1, 2, 3)
	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* MaySkill1{};

	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* MaySkill2{};

	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* MaySkill3{};

	// Cody 스킬 쿨다운 박스 (순서대로 Skill1, 2, 3)
	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* CodySkill1{};

	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* CodySkill2{};

	UPROPERTY(Meta = (BindWidget))
	UCooldownBox* CodySkill3{};
};
