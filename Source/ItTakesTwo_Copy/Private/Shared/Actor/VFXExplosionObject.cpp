// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/VFXExplosionObject.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StatComponent.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"
#include "DrawDebugHelpers.h"

AVFXExplosionObject::AVFXExplosionObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVFXExplosionObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionSphereComponent)
	{
		CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVFXExplosionObject::OnExplosionBeginOverlap);
	}

	if (CollisionBoxComponent)
	{
		CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVFXExplosionObject::OnExplosionBeginOverlap);
	}

}

void AVFXExplosionObject::Tick(float DeltaTime)
{
	if (!bUsing)
	{
		return;
	}

	if (VFXInfo.CollisionInfo.bAttack)
	{
		switch (VFXInfo.CollisionInfo.CollisionShape)
		{
		case EVFXCollisionShape::Sphere:
			if (CollisionSphereComponent &&
				CollisionSphereComponent->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
			{
				DrawDebugSphere(
					GetWorld(),
					CollisionSphereComponent->GetComponentLocation(),
					CollisionSphereComponent->GetScaledSphereRadius(),
					24,
					FColor::Red,
					false,
					0.f,
					0,
					3.f
				);
			}
			break;

		case EVFXCollisionShape::Box:
			if (CollisionBoxComponent &&
				CollisionBoxComponent->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
			{
				DrawDebugBox(
					GetWorld(),
					CollisionBoxComponent->GetComponentLocation(),
					CollisionBoxComponent->GetScaledBoxExtent(),
					CollisionBoxComponent->GetComponentQuat(),
					FColor::Red,
					false,
					0.f,
					0,
					3.f
				);
			}
			break;

		default:
			break;
		}
	}

	Super::Tick(DeltaTime);
}

void AVFXExplosionObject::OnExplosionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !bUsing)
		return;

	if (!OtherActor || OtherActor == this || OtherActor == VFXInfo.OwnerActor)
		return;

	if (!VFXInfo.OwnerActor)
		return;

	const FName TargetName = VFXInfo.OwnerActor->GetTargetName();
	if (TargetName != NAME_None && !OtherActor->ActorHasTag(TargetName))
		return;

	if (!VFXInfo.CollisionInfo.bAttack)
		return;

	const int32 Damage = VFXInfo.CollisionInfo.Damage > 0.f
		? FMath::RoundToInt(VFXInfo.CollisionInfo.Damage)
		: VFXInfo.OwnerActor->GetStatComponent()->GetAttackPower();

	if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
	{
		FHitRequest Request(
			VFXInfo.OwnerActor,
			OtherActor,
			Damage,
			SweepResult.ImpactPoint
		);

		CombatSystem->ProcessHit(Request);
	}
}

void AVFXExplosionObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	Super::UseVFXObject(SpawnInfo);

	if (SpawnInfo.SpawnSoundId.IsNone())
		return;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
		return;

	USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>();
	if (!SoundManager)
		return;

	SoundManager->PlaySFX3D(SpawnInfo.SpawnSoundId, GetActorLocation());
}
void AVFXExplosionObject::FinishVFXObject()
{
	Super::FinishVFXObject();
}

