// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyArbalist_Monster.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"


AToyArbalist_Monster::AToyArbalist_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Standing;	
	DetectRadius = 2000.0f;
	AttackRange = 1000.f;
	MaxIdleTime = 0.8f;
	MoveSpeed = 0.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AToyArbalist_Monster::BeginPlay()
{
	Super::BeginPlay();
	RightHand_WeaponMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
}

void AToyArbalist_Monster::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Fire)
		ProjectileFire();
	
	Super::AnimNotify_MontageEnd();
}

void AToyArbalist_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToyArbalist_Monster::ProjectileFire()
{
	if (!HasAuthority())
		return;

	if (!ProjectileNiagara)
		return;
	
	UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
	if (!PoolSubsystem)
		return;

	const FVector SpawnLocation =
		GetActorLocation()	+ GetActorForwardVector() * SpawnForwardOffset
		+ FVector(0.0f, 0.0f, SpawnUpOffset);
	
	FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateDirectionProjectileLifeTime(
			 this,
			 ProjectileNiagara,
			 ProjectileSpeed,
			 SpawnLocation,
			 GetActorForwardVector(),
			 ProjectileLifeTime
		 );
	SpawnInfo.WithSphereCollision(
		true,
		FName(TEXT("MonsterWeapon")),
		ProjectileDamage,
		ProjectileRadius
	);
	SpawnInfo.WithOverlapExplosion(
		OverlapNiagara,
		0.f
	);
	
	PoolSubsystem->UseVFX_Projectile(SpawnInfo);
}



