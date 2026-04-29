// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/VFXExplosionObject.h"

#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StatComponent.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"


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

	Super::Tick(DeltaTime);
}

void AVFXExplosionObject::OnExplosionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bUsing)
	{
		return;
	}

	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (OtherActor == VFXInfo.OwnerActor)
	{
		return;
	}

	
	if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
	{
		if (Cast<ACodyCharacter>(VFXInfo.OwnerActor))
		{
		}
		else
		{
			FHitRequest Request(GetOwner(), OtherActor, 
				VFXInfo.OwnerActor->GetStatComponent()->GetAttackPower(), 
				SweepResult.ImpactPoint);
			CombatSystem->ProcessHit(Request);
		}
	}
	
	
}


void AVFXExplosionObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	Super::UseVFXObject(SpawnInfo);
	
}

void AVFXExplosionObject::FinishVFXObject()
{
	Super::FinishVFXObject();
}

