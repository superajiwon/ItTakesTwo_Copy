

#include "Actors/Characters/Monsters/MonsterBase.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"
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

float AMonsterBase::GetDetectRadius() const
{
	return DetectRadius;
}

float AMonsterBase::GetAttackRange() const
{
	return AttackRange;
}

void AMonsterBase::OnRep_MonsterState()
{
	// 상태 변경 시 클라에서 애니메이션 처리
}

