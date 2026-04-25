// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyMage_Monster.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/FVFXSpawn_Info.h"
#include "Shared/Struct/HitComp_Info.h"
#include "NiagaraSystem.h"

AToyMage_Monster::AToyMage_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Teleport;
	DetectRadius = 1500.0f;
	AttackRange = 150.f;
	MaxIdleTime = 1.f;
	TargetLocationMap.Add(0, FVector(1940.0f, 0.0f, 100.0f));
	TargetLocationMap.Add(1, FVector(720.0f, -330.0f, 100.0f));
	TargetLocationMap.Add(2, FVector(720.0f, 580.0f, 100.0f));
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("MonsterWeapon"), FVector(0.f, 0.f, 27.0f), FVector(20.f, 20.f, 130.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Niagara(
		TEXT("/Game/VFX/Using/NS_MageProjectile.NS_MageProjectile"));
	if (Niagara.Succeeded())
	{
		ProjectileNiagara = Niagara.Object;
	}
}

void AToyMage_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyMage_Monster::MoveTeleport(AMonsterAIController* MonsterController, FVector PlayerLocation)
{
	Super::MoveTeleport(MonsterController,PlayerLocation);

	++CurrentLocationIndex;
	if (CurrentLocationIndex > 2)
	{
		CurrentLocationIndex = 0;
	}

	FVector Location = GetActorLocation();
	if (TargetLocationMap.Contains(CurrentLocationIndex))
	{
		Location = TargetLocationMap[CurrentLocationIndex];
	}

	const FVector Dir = PlayerLocation - Location;
	const bool bSuccess = TeleportTo(Location, Dir.Rotation());

	if (bSuccess)
	{
		ForceNetUpdate(); // 중요한 변화니 빨리 갱신하라고 재촉하는 함수
	}
}

void AToyMage_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Super::Tick(DeltaTime);
	DrawDebugBox(
		GetWorld(),
		HitBoxComponent->GetComponentLocation(),
		HitBoxComponent->GetScaledBoxExtent(),
		HitBoxComponent->GetComponentQuat(),
		FColor::Red,
		false,
		-1,
		0,
		2.f
	);
}

void AToyMage_Monster::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Fire)
		ProjectileFire();
	
	Super::AnimNotify_MontageEnd();
	
}

void AToyMage_Monster::ProjectileFire()
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
	const FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateDirectionProjectileLifeTimeSphere(
		this,
		ProjectileNiagara,
		ProjectileSpeed,
		SpawnLocation,
		GetActorForwardVector(),
		ProjectileLifeTime,
		true,
		FName(TEXT("MonsterWeapon")),
		ProjectileDamage,
		ProjectileRadius
	);

	PoolSubsystem->UseVFX_Projectile(SpawnInfo);
}



