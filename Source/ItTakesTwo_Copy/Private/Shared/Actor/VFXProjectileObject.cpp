
#include "Shared/Actor/VFXProjectileObject.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"


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
	
	FinishVFXObject();
}

void AVFXProjectileObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	Super::UseVFXObject(SpawnInfo);
	
	ProjectileMovement->InitialSpeed = FMath::Clamp( SpawnInfo.Speed - 100.f,100.f, SpawnInfo.Speed);
	ProjectileMovement->MaxSpeed = SpawnInfo.Speed;
	ProjectileMovement->Velocity = SpawnInfo.Direction.GetSafeNormal() * SpawnInfo.Speed;
	ProjectileMovement->Activate();
}

void AVFXProjectileObject::FinishVFXObject()
{
	if (ProjectileMovement)
	{
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

	if (OtherActor == VFXInfo.OwnedActor)
	{
		return;
	}

	if (VFXInfo.bSpawnOverlapExplosion && VFXInfo.OverlapExplosionNiagara)
	{
		UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
		if (PoolSubsystem)
		{
			const FVFXSpawn_Info ExplosionInfo = FVFXSpawn_Info::CreateExplosionOnce_NoCollision(
				VFXInfo.OwnedActor,
				VFXInfo.OverlapExplosionNiagara,
				GetActorLocation()
			);

			PoolSubsystem->UseVFX_Explosion(ExplosionInfo);
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

