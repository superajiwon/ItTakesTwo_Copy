// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyKnightMorningStar_Monster.generated.h"

class UHitBoxComponent;
class UStaticMeshComponent;
UCLASS()
class ITTAKESTWO_COPY_API AToyKnightMorningStar_Monster : public AWeaponMonsterBase
{
	GENERATED_BODY()

public:
	AToyKnightMorningStar_Monster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitBoxComponent> HitBoxComponent;

	
};
