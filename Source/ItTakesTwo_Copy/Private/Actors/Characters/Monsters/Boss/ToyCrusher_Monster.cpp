// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyCrusher_Monster.h"

#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Map/MapObjectBase.h"
#include "Actors/Map/MapObject_Attackable.h"
#include "Actors/Map/MapObject_Button.h"
#include "Actors/Map/MapObject_Door.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/FVFXSpawn_Info.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"


class UVFXObjectPoolSubsystem;

AToyCrusher_Monster::AToyCrusher_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	

	MoveSpeed = 300.f;

	AIControllerClass = nullptr;
	AutoPossessAI = EAutoPossessAI::Disabled;
	MonsterMoveType = EMonsterMoveType::End;

	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	HitBoxComponent->SetBoxExtent(FVector(130.f, 700.f, 570.f));
	HitBoxComponent->SetRelativeLocation(FVector(80.f, 0.f, 628.f));
	HitBoxComponent->SetCollisionProfileName(FName(TEXT("MonsterWeapon")));
	HitBoxComponent->SetGenerateOverlapEvents(false);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBoxComponent->SetDamage(static_cast<int32>(ImpactDamage));

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&AToyCrusher_Monster::OnHitBoxBeginOverlap
	);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Asset(
		TEXT("/Game/VFX/Using/NS_CrusherImpact.NS_CrusherImpact")
	);

	if (Asset.Succeeded())
	{
		ImpactNiagara = Asset.Object;
	}
}

void AToyCrusher_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyCrusher_Monster::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	if (!bActive)
		return;

	if (!OtherActor || OtherActor == this)
		return;
	if (Cast<AMapObject_Button>(OtherActor))
		OtherActor->Destroy();
	const bool bIsPlayer = Cast<APlayerBase>(OtherActor) != nullptr;
	const bool bIsAttackableMapObject = Cast<AMapObject_Attackable>(OtherActor) != nullptr;
	const bool bIsAttackableDoor = Cast<AMapObject_Door>(OtherActor) != nullptr;

	if (!bIsPlayer && !bIsAttackableMapObject && !bIsAttackableDoor)
		return;

	UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>();
	if (!CombatSystem)
		return;

	const FVector HitLocation =OtherActor->GetActorLocation();

	FHitRequest Request(
		this,
		OtherActor,
		static_cast<int32>(ImpactDamage),
		HitLocation
	);

	CombatSystem->ProcessHit(Request);

	bOverlapedToTarget = true;
}

void AToyCrusher_Monster::AnimNotify_CrusherAttack()
{
	if (!HasAuthority())
		return;

	if (!ImpactNiagara)
		return;

	UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
	if (!PoolSubsystem)
		return;

	const FVector SpawnLocation = GetActorLocation();

	FVFXSpawn_Info SpawnInfo = FVFXSpawn_Info::CreateExplosionOnce(
		this,
		ImpactNiagara,
		SpawnLocation
	);

	SpawnInfo.WithBoxCollision(
	true,
	FName(TEXT("MonsterWeapon")),
	ImpactDamage,
	ImpactExtents
);

	PoolSubsystem->UseVFX_Explosion(SpawnInfo);
}

void AToyCrusher_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
		return;

	if (!bActive)
	{
		const bool bFirstPressed =
			First_Button &&
			First_Button->GetMapObjectState() == EMapObjectState::AfterPressed;

		const bool bSecondPressed =
			Second_Button &&
			Second_Button->GetMapObjectState() == EMapObjectState::AfterPressed;

		if (bFirstPressed && bSecondPressed)
		{
			bActive = true;
			SetMonsterState(EMonsterState::Chase);

			if (HitBoxComponent)
			{
				HitBoxComponent->SetGenerateOverlapEvents(true);
				HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				HitBoxComponent->UpdateOverlaps();
			}
		}
	}

	if (!bActive)
		return;

	const FVector DeltaMove = GetActorForwardVector() * MoveSpeed * DeltaTime;
	AddActorWorldOffset(DeltaMove, true);

	if (GetOverlapToTarget())
	{
		if (GetMonsterState() != EMonsterState::Swing)
		{
			SetMonsterState(EMonsterState::Swing);
		}
	}
	else
	{
		if (GetMonsterState() != EMonsterState::Chase)
		{
			SetMonsterState(EMonsterState::Chase);
		}
	}
}



