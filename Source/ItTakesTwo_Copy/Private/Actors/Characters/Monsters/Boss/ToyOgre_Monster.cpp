// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_HandCollider.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Net/UnrealNetwork.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Struct/FVFXSpawn_Info.h"
#include "NiagaraSystem.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Characters/Managers/CameraManagerActor.h"
#include "Shared/ITTGameInstance.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

AToyOgre_Monster::AToyOgre_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 카메라에 보이는지에 따라 애니메이션 업데이트 결정
	// 보스는 항상으로
	GetMesh()->VisibilityBasedAnimTickOption =
	EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	AIControllerClass = nullptr;
	AutoPossessAI = EAutoPossessAI::Disabled;
	MonsterMoveType = EMonsterMoveType::End;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(
		FName("Monster"), 
		FName("MonsterWeapon"), 
		FVector(110.0f, 0.f, 150.0f), 
		FVector(220.f, 150.0f, 150.f));
	
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
	HitBoxComponent->SetDamage(30);
	HitBoxComponent->CollisionOff();
	HitBoxComponent->bAutoResetEndOverlap = true;
	// 상태 머신 추가
	StateMachine = CreateDefaultSubobject<UToyOgre_StateMachineComponent>(TEXT("StateMachine"));
	
}


void AToyOgre_Monster::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh())
	{
		DefaultMeshRelativeLocation = GetMesh()->GetRelativeLocation();
		DefaultMeshRelativeRotation = GetMesh()->GetRelativeRotation();
	}
	if (HasAuthority())
	{
		SpawnHandColliders();
		StateMachine->Init(this);
		StateMachine->ChangeState(SelectTargetStateClass);
	}
	
	//! 카메라에 보스 추가
	// 맵에 존재하는 카메라 매니저를 찾음
	AActor* CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManagerActor::StaticClass());
	if (ACameraManagerActor* CameraManager = Cast<ACameraManagerActor>(CameraActor))
	{
		CameraManager->AddTarget(this); // 카메라 추적 대상에 보스 추가
	}
}
void AToyOgre_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RepMeshTransform.bUseOverride)
	{
		ApplyMeshTransform();
	}

	if (const UITTGameInstance* GI = GetGameInstance<UITTGameInstance>())
	{
		if (GI->IsGameplayPausedForLoading())
		{
			return;
		}
	}

	if (HasAuthority())
	{
		StateMachine->TickState(DeltaTime);
	}
}


void AToyOgre_Monster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AToyOgre_Monster, ToyOgreState);
	DOREPLIFETIME(AToyOgre_Monster, RepMeshTransform);
}

void AToyOgre_Monster::Multicast_PlayOgreSFX2D_Implementation(FName SoundId)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager =
			GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX2D(SoundId);
		}
	}
}

void AToyOgre_Monster::Multicast_PlayOgreSFX3D_Implementation(FName SoundId)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager =
			GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX3D(SoundId, GetActorLocation());
		}
	}
}


void AToyOgre_Monster::OnRep_ToyOgreState()
{

}

void AToyOgre_Monster::SetToyOgreState(EToyOgreState NewState)
{
	if (!HasAuthority())
		return;

	if (ToyOgreState == NewState)
		return;

	ToyOgreState = NewState;

	OnRep_ToyOgreState();
}

void AToyOgre_Monster::PlayToyOgreMontage(UAnimMontage* Montage)
{
	if (!HasAuthority())
		return;

	Multicast_PlayToyOgreMontage(Montage);
}

void AToyOgre_Monster::Multicast_PlayToyOgreMontage_Implementation(UAnimMontage* Montage)
{
	if (!Montage || !GetMesh())
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst)
		return;

	AnimInst->Montage_Play(Montage);
}

void AToyOgre_Monster::AnimNotify_ToyOgre(FName EventName)
{
	if (!HasAuthority())
		return;

	if (StateMachine)
	{
		StateMachine->HandleAnimNotify(EventName);
	}
}



bool AToyOgre_Monster::RotateToCurrentTarget(float DeltaTime, float RotateSpeed)
{
	TWeakObjectPtr<AActor> Target = GetCurrentTarget();
	if (!Target.IsValid())
		return false;

	const FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	const FRotator TargetRot = FRotationMatrix::MakeFromX(ToTarget).Rotator();

	const FRotator NewRot = FMath::RInterpTo(
		GetActorRotation(),
		FRotator(0.f, TargetRot.Yaw, 0.f),
		DeltaTime,
		RotateSpeed
	);

	SetActorRotation(NewRot);
	return true;
}

void AToyOgre_Monster::SpawnMeteor()
{
	if (!HasAuthority() || !GetWorld() || !MeteorNiagara)
		return;

	UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
	if (!PoolSubsystem)
		return;

	const FVector Center = FVector::ZeroVector;

	for (int32 i = 0; i < MeteorCount; ++i)
	{
		const float Angle = FMath::RandRange(0.f, 2.f * PI);
		const float Distance = FMath::Sqrt(FMath::FRand()) * MeteorSpawnRadius;
		const FVector GroundLocation = Center + FVector(
			FMath::Cos(Angle) * Distance,
			FMath::Sin(Angle) * Distance,
			0.f
		);

		const FVector StartLocation = GroundLocation + FVector(0.f, 0.f, MeteorSpawnHeight);
		const FVector Direction = FVector::DownVector;

		FVFXSpawn_Info SpawnInfo =
	FVFXSpawn_Info::CreateDirectionProjectileLifeTime(
		this,
		MeteorNiagara,
		MeteorSpeed,
		StartLocation,
		FVector::DownVector,
		MeteorLifeTime
	);

		SpawnInfo.WithSphereCollision(true,	FName(TEXT("Meteor")),0.f,50.f);
		SpawnInfo.WithOverlapExplosion(MeteorImpactNiagara, 3.f);
		SpawnInfo.WithOverlapExplosionSphereCollision(FName(TEXT("MonsterWeapon")), MeteorDamage, MeteorCollisionRadius);
		SpawnInfo.AsProjectileTriggerExplosionOnly();
		SpawnInfo.WithSpawnSound(TEXT("Ogre_Explosion"));
		PoolSubsystem->UseVFX_Projectile(SpawnInfo);

	}
}

void AToyOgre_Monster::OnHandBroken(bool IsLeftHand)
{
	if (!HasAuthority())
		return;

	if (bOgreDying)
		return;

	if (AreBothHandsBroken())
	{
		StartHandDeath(IsLeftHand);
		DeactivateHandColliders();

		if (IsLeftHand)
		{
			GetStateMachine()->ChangeState(LeftHandHurtDeathStateClass);
		}
		else
		{
			GetStateMachine()->ChangeState(RightHandHurtDeathStateClass);
		}
		
		//! 카메라 
		AActor* CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManagerActor::StaticClass());
		if (ACameraManagerActor* CameraManager = Cast<ACameraManagerActor>(CameraActor))
		{
			CameraManager->RemoveTarget(this); // 카메라 추적 대상에서 보스 제외
		}
		
		return;
	}

	if (IsLeftHand)
	{
		LeftHandHurt();

		GetWorldTimerManager().SetTimer(
			LeftHandRegenTimer,
			FTimerDelegate::CreateUObject(this, &AToyOgre_Monster::RegenHand, true),
			HandRegenDelay,
			false
		);
	}
	else
	{
		RightHandHurt();

		GetWorldTimerManager().SetTimer(
			RightHandRegenTimer,
			FTimerDelegate::CreateUObject(this, &AToyOgre_Monster::RegenHand, false),
			HandRegenDelay,
			false
		);
	}
}
void AToyOgre_Monster::SetMeshWorldLocationForHole(const FVector& MeshWorldLocation)
{
	if (!HasAuthority() || !GetMesh())
		return;

	RepMeshTransform.WorldLocation = MeshWorldLocation;
	RepMeshTransform.bUseOverride = true;

	ApplyMeshTransform();
	Multicast_ApplyMeshTransform(RepMeshTransform);
}

void AToyOgre_Monster::ResetMeshTransform()
{
	if (!HasAuthority())
		return;

	RepMeshTransform.bUseOverride = false;

	ApplyMeshTransform();
	Multicast_ApplyMeshTransform(RepMeshTransform);
}

void AToyOgre_Monster::StartHoleEnterAtMeshWorldLocation(const FVector& MeshWorldLocation, FName SoundId,
	bool bUse3DSound)
{
	if (!HasAuthority() || !GetMesh())
		return;

	FToyOgre_MeshTransformInfo MeshTransform;
	MeshTransform.WorldLocation = MeshWorldLocation;
	MeshTransform.bUseOverride = true;

	RepMeshTransform = MeshTransform;

	Multicast_StartHoleEnter(
		MeshTransform,
		HoleEnterMontage,
		SoundId,
		MeshWorldLocation,
		bUse3DSound
	);
}


void AToyOgre_Monster::Multicast_StartHoleEnter_Implementation(
	const FToyOgre_MeshTransformInfo& MeshTransform,
	UAnimMontage* Montage,
	FName SoundId,
	FVector SoundLocation,
	bool bUse3DSound
)
{
	RepMeshTransform = MeshTransform;
	ApplyMeshTransform();

	ToyOgreState = EToyOgreState::Hole_Enter;
	OnRep_ToyOgreState();

	if (Montage && GetMesh())
	{
		if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
		{
			AnimInst->Montage_Play(Montage);
		}
	}

	if (!SoundId.IsNone())
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (USoundManagerSubsystem* SoundManager =
				GameInstance->GetSubsystem<USoundManagerSubsystem>())
			{
				if (bUse3DSound)
				{
					SoundManager->PlaySFX3D(SoundId, SoundLocation);
				}
				else
				{
					SoundManager->PlaySFX2D(SoundId);
				}
			}
		}
	}
}


void AToyOgre_Monster::Multicast_ApplyMeshTransform_Implementation(const FToyOgre_MeshTransformInfo& MeshTransform)
{
	RepMeshTransform = MeshTransform;
	ApplyMeshTransform();
}

void AToyOgre_Monster::OnRep_MeshTransform()
{
	ApplyMeshTransform();
}

void AToyOgre_Monster::ApplyMeshTransform()
{
	if (!GetMesh())
		return;

	if (RepMeshTransform.bUseOverride)
	{
		GetMesh()->SetAbsolute(true, false, false);

		GetMesh()->SetWorldLocation(
			RepMeshTransform.WorldLocation,
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
	else
	{
		GetMesh()->SetAbsolute(false, false, false);

		GetMesh()->SetRelativeLocationAndRotation(
			DefaultMeshRelativeLocation,
			DefaultMeshRelativeRotation
		);
	}
}


void AToyOgre_Monster::RightHandHurt() const
{
	switch (ToyOgreState)
	{
		case EToyOgreState::Hole_GrabBothHands:
		{
			GetStateMachine()->ChangeState(RightHandHurtStateClass);
			break;
		}
		case EToyOgreState::Hole_GrabOnlyLeftHand:
		{
			GetStateMachine()->ChangeState(RightHandHurtDeathStateClass);
			break;
		}
		default:
			break;
	}
	
}

void AToyOgre_Monster::LeftHandHurt()
{
	switch (ToyOgreState)
	{
		case EToyOgreState::Hole_GrabBothHands:
		{
			GetStateMachine()->ChangeState(LeftHandHurtStateClass);
			break;
		}
		case EToyOgreState::Hole_GrabOnlyRightHand:
		{
			GetStateMachine()->ChangeState(LeftHandHurtDeathStateClass);
			break;
		}
		default:
			break;
	}
}

void AToyOgre_Monster::SpawnHandColliders()
{
	if (!HandColliderClass || !GetMesh() || !GetWorld())
		return;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	LeftHandCollider = GetWorld()->SpawnActor<AToyOgre_HandCollider>(
		HandColliderClass,
		GetMesh()->GetSocketTransform(FName("LeftHandSocket")),
		Params
	);

	if (LeftHandCollider)
	{
		LeftHandCollider->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("LeftHandSocket")
		);

		LeftHandCollider->InitHand(this, true);
	}

	RightHandCollider = GetWorld()->SpawnActor<AToyOgre_HandCollider>(
		HandColliderClass,
		GetMesh()->GetSocketTransform(FName("RightHandSocket")),
		Params
	);

	if (RightHandCollider)
	{
		RightHandCollider->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("RightHandSocket")
		);

		RightHandCollider->InitHand(this, false);
	}
	DeactivateHandColliders();
}
void AToyOgre_Monster::CompleteLeftHandRecover()
{
	if (!HasAuthority() || bOgreDying)
		return;

	if (RightHandCollider && RightHandCollider->IsBroken())
	{
		StartHandDeath(false);
		return;
	}

	if (LeftHandCollider)
	{
		LeftHandCollider->RegenHand();
	}

	GetStateMachine()->ChangeState(GrabBothHandsStateClass);
}

void AToyOgre_Monster::CompleteRightHandRecover()
{
	if (!HasAuthority() || bOgreDying)
		return;

	if (LeftHandCollider && LeftHandCollider->IsBroken())
	{
		StartHandDeath(true);
		return;
	}

	if (RightHandCollider)
	{
		RightHandCollider->RegenHand();
	}

	GetStateMachine()->ChangeState(GrabBothHandsStateClass);
}


void AToyOgre_Monster::ActivateLeftHandCollider()
{
	if (LeftHandCollider)
	{
		LeftHandCollider->ActivateHand();
	}
}

void AToyOgre_Monster::DeactivateLeftHandCollider()
{
	if (LeftHandCollider)
	{
		LeftHandCollider->DeactivateHand();
	}
}

void AToyOgre_Monster::ActivateRightHandCollider()
{
	if (RightHandCollider)
	{
		RightHandCollider->ActivateHand();
	}
}

void AToyOgre_Monster::DeactivateRightHandCollider()
{
	if (RightHandCollider)
	{
		RightHandCollider->DeactivateHand();
	}
}

void AToyOgre_Monster::ActivateHandColliders()
{
	if (LeftHandCollider)
	{
		LeftHandCollider->ActivateHand();
	}
	if (RightHandCollider)
	{
		RightHandCollider->ActivateHand();
	}
}

void AToyOgre_Monster::DeactivateHandColliders()
{
	if (LeftHandCollider)
	{
		LeftHandCollider->DeactivateHand();
	}
	if (RightHandCollider)
	{
		RightHandCollider->DeactivateHand();
	}
}

void AToyOgre_Monster::RegenHand(bool IsLeftHand)
{
	if (!HasAuthority() || bOgreDying || ToyOgreState == EToyOgreState::Dead)
		return;

	if (IsLeftHand)
	{
		GetStateMachine()->ChangeState(LeftHandRecoverStateClass);
	}
	else
	{
		GetStateMachine()->ChangeState(RightHandRecoverStateClass);
	}
}

bool AToyOgre_Monster::AreBothHandsBroken() const
{
	return LeftHandCollider &&	RightHandCollider &&
		LeftHandCollider->IsBroken() &&	RightHandCollider->IsBroken();
}

void AToyOgre_Monster::StartHandDeath(bool IsLeftHand)
{
		if (!HasAuthority() || bOgreDying)
    		return;
    
    	bOgreDying = true;
    
    	ClearHandRegenTimers();
    	DeactivateHandColliders();
    
    	if (IsLeftHand)
    	{
    		GetStateMachine()->ChangeState(LeftHandHurtDeathStateClass);
    	}
    	else
    	{
    		GetStateMachine()->ChangeState(RightHandHurtDeathStateClass);
    	}
}

void AToyOgre_Monster::ClearHandRegenTimers()
{
	GetWorldTimerManager().ClearTimer(LeftHandRegenTimer);
	GetWorldTimerManager().ClearTimer(RightHandRegenTimer);
}

void AToyOgre_Monster::PlayDeathSequence()
{
	if (!HasAuthority())
		return;

	if (bDeathSequencePlayed)
		return;

	bDeathSequencePlayed = true;

	Multicast_PlayDeathSequence();
	
}

void AToyOgre_Monster::Multicast_PlayDeathSequence_Implementation()
{
	AActor* CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(),	ACameraManagerActor::StaticClass());
	if (ACameraManagerActor* CameraManager = Cast<ACameraManagerActor>(CameraActor))
	{
		CameraManager->SetCinematicMode(true);
	}

	if (!DeathSequenceActor)
		return;
	ULevelSequencePlayer* SequencePlayer = DeathSequenceActor->GetSequencePlayer();
	if (!SequencePlayer)
		return;
	
	SequencePlayer->Play();
}