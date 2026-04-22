// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyArbalist_Monster.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AToyArbalist_Monster : public AWeaponMonsterBase
{
	GENERATED_BODY()

public:
	AToyArbalist_Monster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
