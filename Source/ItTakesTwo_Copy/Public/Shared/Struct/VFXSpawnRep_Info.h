#pragma once

#include "FVFXSpawn_Info.h"
#include "CoreMinimal.h"
#include "VFXSpawnRep_Info.generated.h"


class UNiagaraSystem;
class ACharacterBase;

USTRUCT(BlueprintType)
struct FVFXSpawnRep_Info
{
	GENERATED_BODY()
    
    UPROPERTY()
    bool bUsing = false;
	
	UPROPERTY()
	bool bProjectileDamageOnOverlap = true;

	UPROPERTY()
	bool bExplosionUsesProjectileCollisionInfo = false;
    
	UPROPERTY()
    TObjectPtr<UNiagaraSystem> NiagaraAsset = nullptr;
    
    UPROPERTY()
    EVFXSpawnType VFXType = EVFXSpawnType::End;
    
    UPROPERTY()
    FVector StartLocation = FVector::ZeroVector;
    
    UPROPERTY()
    FRotator StartRotation = FRotator::ZeroRotator;
    
    UPROPERTY()
    FVector Direction = FVector::ZeroVector;
    
    UPROPERTY()
    float Speed = 0.f;
    
    UPROPERTY()
    float LifeTime = 0.f;
    
    UPROPERTY()
    float LifeDistance = 0.f;
    
    UPROPERTY()
    TObjectPtr<ACharacterBase> TargetActor = nullptr;
    
    // 같은 풀 객체를 여러 번 재사용할 때 OnRep가 확실히 다시 반응하게 하는 번호
    UPROPERTY()
    int32 ActivationId = 0;
	
	UPROPERTY()
	FVFXCollision_Info CollisionInfo;

	UPROPERTY()
	bool bSpawnOverlapExplosion = false;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> OverlapExplosionNiagara = nullptr;

	UPROPERTY()
	float OverlapExplosionLifeTime = 0.f;
	
	UPROPERTY()
	FVFXCollision_Info OverlapExplosionCollisionInfo;
};
