// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Monsters/MonsterBase.h"
#include "BossBase.generated.h"

class UHitBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API ABossBase : public AMonsterBase
{
	GENERATED_BODY()

public:
	ABossBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitBoxComponent> HitBoxComponent;
};
