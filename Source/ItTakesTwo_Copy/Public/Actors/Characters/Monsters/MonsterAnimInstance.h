// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public: 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_MontageEnd();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSetting")
	EMonsterState CurrentState{EMonsterState::Idle};
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<AMonsterBase> CachedMonster{nullptr};	
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMoving = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsDead{false};




};
