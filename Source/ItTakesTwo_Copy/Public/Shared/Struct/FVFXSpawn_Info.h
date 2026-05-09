#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "FVFXSpawn_Info.generated.h"

class ACharacterBase;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EVFXSpawnType : uint8
{
	Projectile_Distance,
	Projectile_LifeTime,
	Projectile_Hit,
	Projectile_Homing,
	Explosion_LifeTime,
	Explosion_Once,
	End
};

UENUM(BlueprintType)
enum class EVFXCollisionShape : uint8
{
	None,
	Sphere,
	Box
};

USTRUCT(BlueprintType)
struct FVFXCollision_Info
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVFXCollisionShape CollisionShape = EVFXCollisionShape::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionPresetName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxExtents = FVector::ZeroVector;

public:
	static FVFXCollision_Info NoCollision()
	{
		FVFXCollision_Info Info;
		Info.bAttack = false;
		Info.CollisionShape = EVFXCollisionShape::None;
		Info.CollisionPresetName = NAME_None;
		Info.Damage = 0.f;
		Info.SphereRadius = 0.f;
		Info.BoxExtents = FVector::ZeroVector;
		return Info;
	}

	static FVFXCollision_Info Sphere(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		FVFXCollision_Info Info;
		Info.bAttack = bUseAttack;
		Info.CollisionShape = EVFXCollisionShape::Sphere;
		Info.CollisionPresetName = CollisionName;
		Info.Damage = DamageValue;
		Info.SphereRadius = Radius;
		Info.BoxExtents = FVector::ZeroVector;
		return Info;
	}

	static FVFXCollision_Info Box(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& InBoxExtents
	)
	{
		FVFXCollision_Info Info;
		Info.bAttack = bUseAttack;
		Info.CollisionShape = EVFXCollisionShape::Box;
		Info.CollisionPresetName = CollisionName;
		Info.Damage = DamageValue;
		Info.SphereRadius = 0.f;
		Info.BoxExtents = InBoxExtents;
		return Info;
	}
};

USTRUCT(BlueprintType)
struct FVFXSpawn_Info
{
	GENERATED_BODY()

public:
	static constexpr float DefaultLifeDistance = 1000.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVFXSpawnType VFXType = EVFXSpawnType::Explosion_Once;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacterBase> OwnerActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacterBase> TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVFXCollision_Info CollisionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnOverlapExplosion = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> OverlapExplosionNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OverlapExplosionLifeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bProjectileDamageOnOverlap = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bExplosionUsesProjectileCollisionInfo = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVFXCollision_Info OverlapExplosionCollisionInfo;

	
	
public:
	FVFXSpawn_Info() = default;
	// Target Location으로 발사
	static FVFXSpawn_Info CreateTargetProjectile(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& TargetLoc
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(
			Owner,
			Asset,
			EVFXSpawnType::Projectile_Hit,
			MaxSpeed,
			StartLoc,
			(TargetLoc - StartLoc).GetSafeNormal()
		);

		Info.TargetLocation = TargetLoc;
		Info.LifeDistance = DefaultLifeDistance;
		return Info;
	}

	static FVFXSpawn_Info CreateHomingProjectile(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		TObjectPtr<ACharacterBase> Target
	)
	{
		const FVector TargetLoc = Target ? Target->GetActorLocation() : StartLoc;
		const FVector MoveDir = Target ? (TargetLoc - StartLoc).GetSafeNormal() : FVector::ForwardVector;

		FVFXSpawn_Info Info = MakeBaseProjectile(
			Owner,
			Asset,
			EVFXSpawnType::Projectile_Homing,
			MaxSpeed,
			StartLoc,
			MoveDir
		);

		Info.TargetActor = Target;
		Info.TargetLocation = TargetLoc;
		Info.LifeDistance = DefaultLifeDistance;
		return Info;
	}

	static FVFXSpawn_Info CreateDirectionProjectileLifeTime(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float InLifeTime
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(
			Owner,
			Asset,
			EVFXSpawnType::Projectile_LifeTime,
			MaxSpeed,
			StartLoc,
			MoveDirection
		);

		Info.LifeTime = InLifeTime;
		return Info;
	}

	static FVFXSpawn_Info CreateDirectionProjectileDistance(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection,
		float InLifeDistance
	)
	{
		FVFXSpawn_Info Info = MakeBaseProjectile(
			Owner,
			Asset,
			EVFXSpawnType::Projectile_Distance,
			MaxSpeed,
			StartLoc,
			MoveDirection
		);

		Info.LifeDistance = InLifeDistance;
		return Info;
	}

	static FVFXSpawn_Info CreateExplosionOnce(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc
	)
	{
		FVFXSpawn_Info Info;
		Info.VFXType = EVFXSpawnType::Explosion_Once;
		Info.OwnerActor = Owner;
		Info.NiagaraAsset = Asset;
		Info.StartLocation = SpawnLoc;
		Info.CollisionInfo = FVFXCollision_Info::NoCollision();
		return Info;
	}

	static FVFXSpawn_Info CreateExplosionLifeTime(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		const FVector& SpawnLoc,
		float InLifeTime
	)
	{
		FVFXSpawn_Info Info = CreateExplosionOnce(Owner, Asset, SpawnLoc);
		Info.VFXType = EVFXSpawnType::Explosion_LifeTime;
		Info.LifeTime = InLifeTime;
		return Info;
	}

	FVFXSpawn_Info& WithSphereCollision(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		CollisionInfo = FVFXCollision_Info::Sphere(
			bUseAttack,
			CollisionName,
			DamageValue,
			Radius
		);

		return *this;
	}

	FVFXSpawn_Info& WithBoxCollision(
		bool bUseAttack,
		FName CollisionName,
		float DamageValue,
		const FVector& BoxExtents
	)
	{
		CollisionInfo = FVFXCollision_Info::Box(
			bUseAttack,
			CollisionName,
			DamageValue,
			BoxExtents
		);

		return *this;
	}

	FVFXSpawn_Info& WithOverlapExplosion(
		TObjectPtr<UNiagaraSystem> ExplosionNiagara,
		float ExplosionLifeTime
	)
	{
		bSpawnOverlapExplosion = true;
		OverlapExplosionNiagara = ExplosionNiagara;
		OverlapExplosionLifeTime = ExplosionLifeTime;
		return *this;
	}

	FVFXSpawn_Info& AsProjectileTriggerExplosionOnly()
	{
		bProjectileDamageOnOverlap = false;
		bExplosionUsesProjectileCollisionInfo = true;
		return *this;
	}
	
	FVFXSpawn_Info& WithoutCollision()
	{
		CollisionInfo = FVFXCollision_Info::NoCollision();
		return *this;
	}

	FVFXSpawn_Info& WithOverlapExplosionCollision(const FVFXCollision_Info& InCollisionInfo)
	{
		OverlapExplosionCollisionInfo = InCollisionInfo;
		return *this;
	}

	FVFXSpawn_Info& WithOverlapExplosionSphereCollision(
		FName CollisionName,
		float DamageValue,
		float Radius
	)
	{
		OverlapExplosionCollisionInfo = FVFXCollision_Info::Sphere(
			true,
			CollisionName,
			DamageValue,
			Radius
		);

		return *this;
	}

	
private:
	static FVFXSpawn_Info MakeBaseProjectile(
		TObjectPtr<ACharacterBase> Owner,
		TObjectPtr<UNiagaraSystem> Asset,
		EVFXSpawnType SpawnType,
		float MaxSpeed,
		const FVector& StartLoc,
		const FVector& MoveDirection
	)
	{
		FVFXSpawn_Info Info;
		Info.VFXType = SpawnType;
		Info.OwnerActor = Owner;
		Info.NiagaraAsset = Asset;
		Info.StartLocation = StartLoc;
		Info.Direction = MoveDirection.GetSafeNormal();
		Info.StartRotation = Info.Direction.Rotation();
		Info.Speed = MaxSpeed;
		Info.CollisionInfo = FVFXCollision_Info::NoCollision();
		return Info;
	}
};