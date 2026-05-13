// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyMage_Monster.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/FVFXSpawn_Info.h"
#include "Shared/Struct/HitComp_Info.h"
#include "NiagaraSystem.h"
#include "Actors/Characters/Monsters/Teleport_TargetPoint.h"
#include "Shared/Subsystems/TargetPointSubsystem.h"
#include "UObject/ConstructorHelpers.h"


AToyMage_Monster::AToyMage_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Teleport;
	DetectRadius = 1500.0f;
	AttackRange = 150.f;
	MaxIdleTime = 2.f;
	// TargetLocationMap.Add(0, FVector(1940.0f, 0.0f, 100.0f));
	// TargetLocationMap.Add(1, FVector(720.0f, -330.0f, 100.0f));
	// TargetLocationMap.Add(2, FVector(720.0f, 580.0f, 100.0f));
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("MonsterWeapon"), FVector(0.f, 0.f, 27.0f), FVector(20.f, 20.f, 130.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
	HitBoxComponent->SetDamage(30);
	HitBoxComponent->CollisionOff();
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Niagara(
		TEXT("/Game/VFX/Using/NS_MageProjectile.NS_MageProjectile"));
	if (Niagara.Succeeded())
	{
		ProjectileNiagara = Niagara.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> OverlapAsset(
		TEXT("/Game/VFX/Using/NS_MageProjectileEnd.NS_MageProjectileEnd"));
	if (OverlapAsset.Succeeded())
	{
		OverlapNiagara = OverlapAsset.Object;
	}
	
}
void AToyMage_Monster::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		return;

	UTargetPointSubsystem* Subsystem = GetWorld()->GetSubsystem<UTargetPointSubsystem>();
	if (Subsystem)
	{
		TArray<TObjectPtr<ATeleport_TargetPoint>> PointList = Subsystem->GetTeleportTransform();
		
		for (auto& Point : PointList)
		{
			TargetLocationMap.Add(Point->GetIndex(), Point->GetActorTransform());
		}

		if (FTransform* FoundTransform = TargetLocationMap.Find(CurrentLocationIndex))
		{
			SetActorTransform(*FoundTransform);
			++CurrentLocationIndex;
		}
	}
}

void AToyMage_Monster::MoveTeleport(AMonsterAIController* MonsterController, FVector PlayerLocation)
{
	Super::MoveTeleport(MonsterController,PlayerLocation);

	if (!HasAuthority() || TargetLocationMap.Num() == 0)
		return;

	CurrentLocationIndex = (CurrentLocationIndex + 1) % TargetLocationMap.Num();

	const FTransform* FoundTransform = TargetLocationMap.Find(CurrentLocationIndex);
	if (!FoundTransform)
		return;

	const FVector NewLocation = FoundTransform->GetLocation();

	FVector Dir = PlayerLocation - NewLocation;
	Dir.Z = 0.f;

	const FRotator NewRotation = Dir.IsNearlyZero()	? GetActorRotation() : Dir.Rotation();

	const bool bSuccess = TeleportTo(NewLocation, NewRotation, false, true);

	if (bSuccess)
	{
		ForceNetUpdate();
	}
}

void AToyMage_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// DrawDebugBox(
	// 	GetWorld(),
	// 	HitBoxComponent->GetComponentLocation(),
	// 	HitBoxComponent->GetScaledBoxExtent(),
	// 	HitBoxComponent->GetComponentQuat(),
	// 	FColor::Red,
	// 	false,
	// 	-1,
	// 	0,
	// 	2.f
	// );
}

void AToyMage_Monster::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Fire)
	{
		ProjectileFire();
	}
	else if (MonsterState == EMonsterState::Swing)
	{
		HitBoxComponent->CollisionOff();
		HitBoxComponent->ClearHitRecords();
	}
	
	Super::AnimNotify_MontageEnd();
	
}

void AToyMage_Monster::AnimNotify_CollisionOn()
{
	if (MonsterState == EMonsterState::Swing)
		HitBoxComponent->CollisionOn();
	Super::AnimNotify_CollisionOn();
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
	//
	//
	// FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateHomingProjectile(
	// this,
	// ProjectileNiagara,
	// ProjectileSpeed,
	// SpawnLocation,
	// Cast<AMonsterAIController>(Controller)->FindNearestPlayer()
	// );
	// SpawnInfo.WithSphereCollision(
	// 	true,
	// 	FName(TEXT("MonsterWeapon")),
	// 	ProjectileDamage,
	// 	ProjectileRadius
	// );
	// SpawnInfo.WithOverlapExplosion(
	// 	OverlapNiagara,
	// 	0.f
	// );
	//
	// PoolSubsystem->UseVFX_Projectile(SpawnInfo);
	 FVector ForwardDir = GetActorForwardVector();
	 const TArray<FVector> ProjectileDirections =
	 {
	 	ForwardDir,
	 	FRotator(0.f, -45.f, 0.f).RotateVector(ForwardDir),
	 	FRotator(0.f, 45.f, 0.f).RotateVector(ForwardDir)
	 };
	 for (const FVector& Direction : ProjectileDirections)
	 {
	 	FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateDirectionProjectileLifeTime(this,ProjectileNiagara, ProjectileSpeed,SpawnLocation,	Direction, ProjectileLifeTime);
	 	SpawnInfo.WithSphereCollision(true,	FName(TEXT("MonsterWeapon")), ProjectileDamage, ProjectileRadius);
	 	SpawnInfo.WithOverlapExplosion(OverlapNiagara,0.f	);
	 	PoolSubsystem->UseVFX_Projectile(SpawnInfo);
	 }

}
