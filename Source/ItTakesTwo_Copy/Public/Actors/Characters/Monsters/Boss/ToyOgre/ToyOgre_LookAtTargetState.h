// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToyOgre_StateBase.h"
#include "ToyOgre_LookAtTargetState.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API UToyOgre_LookAtTargetState : public UToyOgre_StateBase
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};
