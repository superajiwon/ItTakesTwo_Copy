// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponMonsterBase.h"
#include "ToyArbalist_Monster.generated.h"

class UNiagaraSystem;

UCLASS()
class ITTAKESTWO_COPY_API AToyArbalist_Monster : public AWeaponMonsterBase
{
	GENERATED_BODY()

public:
	AToyArbalist_Monster();

protected:
	virtual void BeginPlay() override;
	virtual void AnimNotify_MontageEnd() override;
public:
	virtual void Tick(float DeltaTime) override;
	
private:
	void ProjectileFire();
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack_Niagara")
	TObjectPtr<UNiagaraSystem> ProjectileNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack_Niagara")
	TObjectPtr<UNiagaraSystem> OverlapNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileLifeTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ProjectileRadius = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float SpawnForwardOffset = 120.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float SpawnUpOffset = 0.0f;

};
