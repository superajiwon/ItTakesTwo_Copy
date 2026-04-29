
#include "Shared/Actor/VFXProjectileObject.h"

#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"



AVFXProjectileObject::AVFXProjectileObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 800.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

}

void AVFXProjectileObject::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionSphereComponent)
	{
		CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVFXProjectileObject::OnProjectileBeginOverlap);
	}
	if (CollisionBoxComponent)
	{
		CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVFXProjectileObject::OnProjectileBeginOverlap);
	}
	
}

void AVFXProjectileObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	Super::UseVFXObject(SpawnInfo);
	
	if (!HasAuthority() || !ProjectileMovement)
	{
		return;
	}

	
	ProjectileMovement->InitialSpeed = FMath::Clamp( SpawnInfo.Speed - 100.f,100.f, SpawnInfo.Speed);
	ProjectileMovement->MaxSpeed = SpawnInfo.Speed;
	ProjectileMovement->Velocity = SpawnInfo.Direction.GetSafeNormal() * SpawnInfo.Speed;
	ProjectileMovement->Activate();
	
	if (SpawnInfo.VFXType == EVFXSpawnType::Projectile_Homing && IsValid(SpawnInfo.TargetActor))
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = SpawnInfo.TargetActor->GetRootComponent();
		ProjectileMovement->HomingAccelerationMagnitude = SpawnInfo.Speed * 6.f;
		
		// 나중에 혹시 Target이 죽거나 하면 직진하도록 추가해야할지도
	}
	
	
}

void AVFXProjectileObject::FinishVFXObject()
{
	if (HasAuthority() && ProjectileMovement)
	{
		ProjectileMovement->bIsHomingProjectile = false;
		ProjectileMovement->HomingTargetComponent = nullptr;
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
	}

	Super::FinishVFXObject();
	
}

void AVFXProjectileObject::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bUsing)
		return;
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (OtherActor == VFXInfo.OwnerActor)
	{
		return;
	}

	if (VFXInfo.bSpawnOverlapExplosion && VFXInfo.OverlapExplosionNiagara)
	{
		UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
		if (PoolSubsystem)
		{
			const FVFXSpawn_Info ExplosionInfo = FVFXSpawn_Info::CreateExplosionOnce(
				VFXInfo.OwnerActor,
				VFXInfo.OverlapExplosionNiagara,
				GetActorLocation()
			);
			PoolSubsystem->UseVFX_Explosion(ExplosionInfo);
			
			if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
			{
				if (Cast<ACodyCharacter>(VFXInfo.OwnerActor))
				{
					
				}
				else
				{
					FHitRequest Request(GetOwner(), OtherActor, VFXInfo.OwnerActor->GetStatComponent()->GetAttackPower(), SweepResult.ImpactPoint);
					CombatSystem->ProcessHit(Request);
				}
			}
		}
	}

	FinishVFXObject();
}

void AVFXProjectileObject::Tick(float DeltaTime)
{
	if (!bUsing)
		return;
	
	Super::Tick(DeltaTime);
	

	
}

