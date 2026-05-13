// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyShielder_Monster.generated.h"

class UHitBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API AToyShielder_Monster : public AWeaponMonsterBase
{
	GENERATED_BODY()

public:
	AToyShielder_Monster();

protected:
	virtual void BeginPlay() override;
	virtual void AnimNotify_CollisionOn() override;
	virtual void AnimNotify_DeadMotionEnd() override;
	virtual void AnimNotify_MontageEnd() override;
public:
	virtual void Tick(float DeltaTime) override;

	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitBoxComponent> HitBoxComponent;

};
