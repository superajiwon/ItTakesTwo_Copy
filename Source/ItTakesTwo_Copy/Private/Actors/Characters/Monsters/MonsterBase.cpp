

#include "Actors/Characters/Monsters/MonsterBase.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"
#include "Actors/Characters/Monsters/MonsterAnimInstance.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	bPlayingMontage = false;
	bOverlapedToTarget = false;
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
		return;

	MonsterController->NotifyAttackAnimationFinished();
}

