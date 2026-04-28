// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyMage_Monster.generated.h"

class UNiagaraSystem;
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
	virtual void AnimNotify_MontageEnd() override;
	
	
private:
	void ProjectileFire();

	
private:
	TMap<int32, FVector> TargetLocationMap;// 순간이동 위치를 담은 배열
	int32 CurrentLocationIndex{0};
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHitBoxComponent> HitBoxComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack_Niagara")
	TObjectPtr<UNiagaraSystem> ProjectileNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack_Niagara")
	TObjectPtr<UNiagaraSystem> OverlapNiagara;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileSpeed = 1200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileLifeTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float SpawnForwardOffset = 120.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float SpawnUpOffset = 0.0f;

};
