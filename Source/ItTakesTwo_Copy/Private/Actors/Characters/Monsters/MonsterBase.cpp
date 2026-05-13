

#include "Actors/Characters/Monsters/MonsterBase.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"
#include "Actors/Characters/Monsters/MonsterAnimInstance.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/HPComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	ACharacter::SetReplicateMovement(true);
	
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterBody"));
	this->Tags.AddUnique(TEXT("Monster"));
	SetTargetName(TEXT("Player"));
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = 2;
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
	AnimInstance = GetMesh() ? Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance()) : nullptr;
}

void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetHPComponent()->GetIsDead())
	{
		SetMonsterState(EMonsterState::Dead);
		bDead = true;
		if (AnimInstance)
		{
			AnimInstance->CurrentState = EMonsterState::Dead;
		}
	}

	// const FString ConStr = (GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetNetMode()==ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"));
	// const FString LogStr = FString::Printf(TEXT("%s\nHP : %f "), *ConStr, GetHPComponent()->GetCurHP());
	// DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 100.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}

void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBase, MonsterState);
}



void AMonsterBase::SetMonsterState(EMonsterState NewState)
{
	if (!HasAuthority())
	{
		return;
	}

	if (MonsterState == NewState)
	{
		return;
	}

	MonsterState = NewState;
	OnRep_MonsterState();
}

void AMonsterBase::SetDetectPlayer(bool bNewDetectPlayer)
{
	AnimInstance->bIsDetect = bNewDetectPlayer;
}

bool AMonsterBase::GetMontagePlayingState() const
{
	return bPlayingMontage;
}

float AMonsterBase::GetDetectRadius() const
{
	return DetectRadius;
}

float AMonsterBase::GetAttackRange() const
{
	return AttackRange;
}

void AMonsterBase::MontagePlay()
{
	if (!AnimInstance)
		AnimInstance = GetMesh() ? Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance()) : nullptr;
	
	if (!AnimInstance)
		return;
	
	UAnimMontage* MontageToPlay{nullptr};

	switch (MonsterState)
	{
	case EMonsterState::Attack:
		{
			MontageToPlay =	AttackMontage;
			break;
		}
	case EMonsterState::Swing:
		{
			MontageToPlay = SwingMontage;
			break;
		}
	case EMonsterState::Fire:
		{
			MontageToPlay = FireMontage;
			break;
		}
	case EMonsterState::TeleportEnter:
		{
			MontageToPlay = TeleportEnterMontage;
			break;
		}
	case EMonsterState::TeleportExit:
		{
			MontageToPlay = TeleportExitMontage;
			break;
		}
	case EMonsterState::Detect:
		{
			MontageToPlay = DetectMontage;
			break;
		}
	case EMonsterState::Dead:
		{
			MontageToPlay = DeadMontage;
			break;
		}
		default:
			break;
	}
	if (!MontageToPlay)
		return;
	
	const float PlayResult = AnimInstance->Montage_Play(MontageToPlay);
	if (PlayResult > 0.0f)
	{
		bPlayingMontage = true;
	}
}

void AMonsterBase::OnRep_MonsterState()
{
	// 상태 변경 시 클라에서 애니메이션 처리
	MontagePlay();
}

void AMonsterBase::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Dead)
	{
		return;
	}
	bPlayingMontage = false;
	bOverlapedToTarget = false;
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
		return;
	
	MonsterController->NotifyAttackAnimationFinished();
}

void AMonsterBase::AnimNotify_CollisionOn()
{
}

void AMonsterBase::AnimNotify_DeadMotionEnd()
{
	
	AnimInstance->Montage_Stop(0.f);
	GetMesh()->SetVisibility(false, true);
	SetActorEnableCollision(false);

	if (HasAuthority())
	{
		Destroy();
	}
}

