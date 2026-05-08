
#include "Actors/Characters/Players/Cody/CodyAnimInstance.h"
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Actors/Characters/Players/Cody/CodyUltimateBox.h"
#include "Components/StatComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Struct/FVFXSpawn_Info.h"


class UVFXObjectPoolSubsystem;

void UCodyAnimInstance::AnimNotify_DashOn()
{
	Super::AnimNotify_DashOn();
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	// 애니메이션 타이밍에 맞춰서 정확하게 코디를 순간이동시킵니다.
	Owner->CodyTeleport(Owner->TeleportLength);

	if (Owner->GetLocalRole() < ROLE_Authority)
	{
		Owner->Server_CodyTeleport();
	}
}

void UCodyAnimInstance::AnimNotify_SpecialOn()
{
	Super::AnimNotify_SpecialOn();
	
	// 코디 얼음 발사체
	if (!ProjectileNiagara) return;
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
	if (!PoolSubsystem) return;
	
	FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateDirectionProjectileLifeTime(
		Owner,
		ProjectileNiagara,
		ProjectileSpeed,
		Owner->SpecialProjectilePoint->GetComponentLocation(),
		Owner->GetActorForwardVector(),
		ProjectileLifeTime
	);
	
	SpawnInfo.WithSphereCollision(
		true,
		FName(TEXT("PlayerWeapon")),
		Owner->GetStatComponent()->GetAttackPower(),
		ProjectileRadius
	);
	
	SpawnInfo.WithOverlapExplosion(
		OverlapNiagara,
		0.f
	);
	
	PoolSubsystem->UseVFX_Projectile(SpawnInfo);
}

void UCodyAnimInstance::AnimNotify_UltimateOn()
{
	Super::AnimNotify_UltimateOn();
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	int32 RandDamage = FMath::RandRange(5, 18);
	Owner->UltimateCollision->SetDamage(RandDamage);
	Owner->UltimateCollision->CollisionOn();
}

