// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToyOgre_StateBase.h"
#include "ToyOgre_TelegraphState.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API UToyOgre_TelegraphState : public UToyOgre_StateBase
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;

	
private:
	UPROPERTY()
	float TelegraphMaxTime{3.f};
	
	UPROPERTY()
	float TelegraphCurrentTime{0.f};
	
};
