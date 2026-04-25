// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyMage_Monster.generated.h"

class UHitBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API AToyMage_Monster : public AWeaponMonsterBase
{
	GENERATED_BODY()

public:
	AToyMage_Monster();

protected:
	virtual void BeginPlay() override;
	virtual void MoveTeleport(AMonsterAIController* MonsterController, FVector PlayerLocation) override;
public:
	virtual void Tick(float DeltaTime) override;

	
	
private:
	TMap<int32, FVector> TargetLocationMap;// 순간이동 위치를 담은 배열
	int32 CurrentLocationIndex{0};
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitBoxComponent> HitBoxComponent;

	
};
