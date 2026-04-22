// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBase.h"
#include "ToyOgre_Monster.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AToyOgre_Monster : public ABossBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AToyOgre_Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
