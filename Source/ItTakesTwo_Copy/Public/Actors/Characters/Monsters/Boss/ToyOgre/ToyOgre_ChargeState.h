// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToyOgre_StateBase.h"
#include "ToyOgre_ChargeState.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UToyOgre_ChargeState : public UToyOgre_StateBase
{
	GENERATED_BODY()
public:
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
	
	
private:
	float RushSpeed{300.f};
	
};