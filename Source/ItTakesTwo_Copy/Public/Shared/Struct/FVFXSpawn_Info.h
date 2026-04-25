#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "FVFXSpawn_Info.generated.h"

#define MAX_DISTANCE 1000.f

UENUM()
enum class EAttackType : uint8
{
	Projectile_Distance,
	Projectile_LifeTime,
	Projectile_Hit,
	Explosion_LifeTime,
	Explosion_Once,
	End
};


class ACharacterBase;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FVFXSpawn_Info
{
	GENERATED_BODY()
	
public:
	EAttackType VFXType{EAttackType::Explosion_Once};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraAsset{nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacterBase> OwnedActor{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartLocation{FVector::Zero()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation{FVector::Zero()};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction{FVector::Zero()};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartRotation{FRotator::ZeroRotator};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life_Distance{0.f}; // 이만큼 이동하면 사라져라
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life_Time{0.f}; // 이만큼 지나면 사라져라
	
	// 유도를 위한 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacterBase> TargetActor{nullptr};
	
	// 공격을 위한 정보
	UPROPERTY(BlueprintReadOnly )
	FName CollisionPresetName{NAME_None};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttack{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseBoxCollision{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Extents{FVector::Zero()};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius{0.0f};
	
	
	// Projectile 충돌 했을 때 소환되는 이펙트용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnOverlapExplosion{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> OverlapExplosionNiagara{nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OverlapExplosionLifeTime{0.0f};
	
public:
	FVFXSpawn_Info() = default;
	
	// Projectile이 부딪혔을 때 소환하기 위함
	FVFXSpawn_Info& CreateOverlapExplosion(
	TObjectPtr<UNiagaraSystem> ExplosionNiagara,
	float ExplosionLifeTime)
	{
		bSpawnOverlapExplosion = true;
		OverlapExplosionNiagara = ExplosionNiagara;
		OverlapExplosionLifeTime = ExplosionLifeTime;
		return *this;
	}
	
	// -----------------------------------------------------------
	// 타겟 위치로 발사 - Collision 없음
	static FVFXSpawn_Info CreateTargetProjectile_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& TargetLoc
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,EAttackType::Projectile_Hit,	MaxSpeed,StartLoc,(TargetLoc - StartLoc).GetSafeNormal());
		Info.TargetLocation = TargetLoc;
		Info.bAttack = false;
		Info.bUseBoxCollision = false;
		Info.CollisionPresetName = NAME_None;
		Info.Damage = 0.f;
		Info.Extents = FVector::ZeroVector;
		Info.SphereRadius = 0.f;
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}

	// -----------------------------------------------------------
	// 유도탄 - Collision 없음
	static FVFXSpawn_Info CreateHomingProjectile_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		TObjectPtr<ACharacterBase> Target
	)
	{
		const FVector TargetLoc = Target ? Target->GetActorLocation() : StartLoc;
		const FVector MoveDir = Target ? (TargetLoc - StartLoc).GetSafeNormal() : FVector::ForwardVector;
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,	EAttackType::Projectile_Hit,MaxSpeed,StartLoc,MoveDir	);
		Info.TargetActor = Target;
		Info.TargetLocation = TargetLoc;
		Info.bAttack = false;
		Info.bUseBoxCollision = false;
		Info.CollisionPresetName = NAME_None;
		Info.Damage = 0.f;
		Info.Extents = FVector::ZeroVector;
		Info.SphereRadius = 0.f;
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}

	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - LifeTime 기반 - Collision 없음
	static FVFXSpawn_Info CreateDirectionProjectileLifeTime_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeTime
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,	EAttackType::Projectile_LifeTime,	MaxSpeed,	StartLoc,	MoveDirection);
		Info.Life_Time = LifeTime;
		Info.bAttack = false;
		Info.bUseBoxCollision = false;
		Info.CollisionPresetName = NAME_None;
		Info.Damage = 0.f;
		Info.Extents = FVector::ZeroVector;
		Info.SphereRadius = 0.f;
		return Info;
	}

	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - 거리 기반 - Collision 없음
	static FVFXSpawn_Info CreateDirectionProjectileDistance_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeDistance
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,	EAttackType::Projectile_Distance,	MaxSpeed,	StartLoc,MoveDirection);
			Info.Life_Distance = LifeDistance;
			Info.bAttack = false;
			Info.bUseBoxCollision = false;
			Info.CollisionPresetName = NAME_None;
			Info.Damage = 0.f;
			Info.Extents = FVector::ZeroVector;
			Info.SphereRadius = 0.f;
			return Info;
		}
	
	
	// Collision 있음
	// -----------------------------------------------------------
	// 타겟 위치로 발사(Fvector Location) - Sphere Collision
	static FVFXSpawn_Info CreateTargetProjectileSphere(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& TargetLoc,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset, EAttackType::Projectile_Hit, MaxSpeed, StartLoc,
			(TargetLoc - StartLoc).GetSafeNormal());
		Info.TargetLocation = TargetLoc;
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}
	
	// -----------------------------------------------------------
	// 타겟 위치로 발사(Fvector Location) - Box Collision
	static FVFXSpawn_Info CreateTargetProjectileBox(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& TargetLoc,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset, EAttackType::Projectile_Hit,	MaxSpeed, StartLoc,
			(TargetLoc - StartLoc).GetSafeNormal());
		Info.TargetLocation = TargetLoc;
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}
	
	// -----------------------------------------------------------
	// 유도탄(TargetActor) - Sphere Collision
	static FVFXSpawn_Info CreateHomingProjectileSphere(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		TObjectPtr<ACharacterBase> Target,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius )
	{
		const FVector TargetLoc = Target ? Target->GetActorLocation() : StartLoc;
		const FVector MoveDir = Target ? (TargetLoc - StartLoc).GetSafeNormal() : FVector::ForwardVector;
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,	EAttackType::Projectile_Hit, MaxSpeed,	StartLoc,MoveDir);
		Info.TargetActor = Target;
		Info.TargetLocation = TargetLoc;
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}
	
	// -----------------------------------------------------------
	// 유도탄(TargetActor)- Box Collision
	static FVFXSpawn_Info CreateHomingProjectileBox(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		TObjectPtr<ACharacterBase> Target,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents	)
	{
		const FVector TargetLoc = Target ? Target->GetActorLocation() : StartLoc;
		const FVector MoveDir = Target ? (TargetLoc - StartLoc).GetSafeNormal() : FVector::ForwardVector;
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,	EAttackType::Projectile_Hit,MaxSpeed,StartLoc,MoveDir);
		Info.TargetActor = Target;
		Info.TargetLocation = TargetLoc;
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		Info.Life_Distance = MAX_DISTANCE;
		return Info;
	}
	
	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - LifeTime 기반 - Sphere Collision
	static FVFXSpawn_Info CreateDirectionProjectileLifeTimeSphere(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeTime,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,EAttackType::Projectile_LifeTime,	MaxSpeed,StartLoc,	MoveDirection);
		Info.Life_Time = LifeTime;
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		return Info;
	}
	
	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - LifeTime 기반 - Box Collision
	static FVFXSpawn_Info CreateDirectionProjectileLifeTimeBox(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeTime,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,EAttackType::Projectile_LifeTime,	MaxSpeed,StartLoc,MoveDirection);
		Info.Life_Time = LifeTime;
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		return Info;
	}
	
	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - 거리 기반 - Sphere Collision
	static FVFXSpawn_Info CreateDirectionProjectileDistanceSphere(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeDistance,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,EAttackType::Projectile_Distance,	MaxSpeed,StartLoc,MoveDirection	);
		Info.Life_Distance = LifeDistance;
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		return Info;
	}

	// -----------------------------------------------------------
	// 지정한 방향으로 직진 - 거리 기반 - Box Collision
	static FVFXSpawn_Info CreateDirectionProjectileDistanceBox(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float LifeDistance,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(Owner,Asset,EAttackType::Projectile_Distance,MaxSpeed,StartLoc,MoveDirection);
		Info.Life_Distance = LifeDistance;
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		return Info;
	}
	
	// -----------------------------------------------------------
	// -----------------------------------------------------------
	// 폭발 - 한 번 실행, 충돌 없음
	static FVFXSpawn_Info CreateExplosionOnce_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc
	)
	{
		FVFXSpawn_Info Info;
		Info.VFXType = EAttackType::Explosion_Once;
		Info.OwnedActor = Owner;
		Info.NiagaraAsset = Asset;
		Info.StartLocation = SpawnLoc;
		return Info;
	}
	
	// 폭발 - LifeTime 기반
	static FVFXSpawn_Info CreateExplosionLifeTime_NoCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		float LifeTime
	)
	{
		FVFXSpawn_Info Info = CreateExplosionOnce_NoCollision(Owner, Asset, SpawnLoc);
		Info.VFXType = EAttackType::Explosion_LifeTime;
		Info.Life_Time = LifeTime;
		return Info;
	}

	// 폭발 - 한 번 실행 - Sphere Collision
	static FVFXSpawn_Info CreateExplosionOnce_SphereCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXSpawn_Info Info = CreateExplosionOnce_NoCollision(Owner, Asset, SpawnLoc);
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		return Info;
	}
	
	// 폭발 - LifeTime 기반 - Sphere Collision
	static FVFXSpawn_Info CreateExplosionLifeTime_SphereCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		float LifeTime,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXSpawn_Info Info = CreateExplosionLifeTime_NoCollision(Owner, Asset, SpawnLoc, LifeTime);
		Info.SetSphereCollision(bUseAttack, CollisionName, DamageValue, Radius);
		return Info;
	}
	
	// 폭발 - 한 번 실행 - Box Collision
	static FVFXSpawn_Info CreateExplosionOnce_BoxCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		FVFXSpawn_Info Info = CreateExplosionOnce_NoCollision(Owner, Asset, SpawnLoc);
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		return Info;
	}
	
	// 폭발 - LifeTime 기반 - Box Collision
	static FVFXSpawn_Info CreateExplosionLifeTime_BoxCollision(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		float LifeTime,
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		FVFXSpawn_Info Info = CreateExplosionLifeTime_NoCollision(Owner, Asset, SpawnLoc, LifeTime);
		Info.SetBoxCollision(bUseAttack, CollisionName, DamageValue, BoxExtents);
		return Info;
	}
	
private:
	static FVFXSpawn_Info MakeBaseProjectile(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		EAttackType AttackType,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection
	)
	{
		FVFXSpawn_Info Info;

		Info.VFXType = AttackType;
		Info.OwnedActor = Owner;
		Info.NiagaraAsset = Asset;
		Info.StartLocation = StartLoc;
		Info.Direction = MoveDirection.GetSafeNormal();
		Info.StartRotation = Info.Direction.Rotation();
		Info.Speed = MaxSpeed;
		return Info;
	}

	void SetSphereCollision(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		bAttack = bUseAttack;
		CollisionPresetName = CollisionName;
		Damage = DamageValue;
		bUseBoxCollision = false;
		SphereRadius = Radius;
		Extents = FVector::ZeroVector;
	}

	void SetBoxCollision(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		bAttack = bUseAttack;
		CollisionPresetName = CollisionName;
		Damage = DamageValue;
		bUseBoxCollision = true;
		Extents = BoxExtents;
		SphereRadius = 0.f;
	}
	
	

};
