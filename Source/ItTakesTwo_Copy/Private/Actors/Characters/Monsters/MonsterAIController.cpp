// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/MonsterAIController.h"

#include "Actors/Characters/Monsters/MonsterBase.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Kismet/GameplayStatics.h"


AMonsterAIController::AMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = (FVector(1940.0f, 0.0f, 100.0f));
	
	if (CachedMonster)
	{
		
		DetectRadius = CachedMonster->GetDetectRadius();
		AttackRange = CachedMonster->GetAttackRange();
		MonsterMoveType = CachedMonster->GetMoveType();
		MaxIdleTime = CachedMonster->GetMaxIdleTime();
	}
	
}


void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	CachedMonster = Cast<AMonsterBase>(InPawn);
	
	if (!CachedMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("No CachedMonster"));
		return;
	}

}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
	{
		return;
	}

	if (!CachedMonster)
	{
		return;
	}

	if (MonsterMoveType == EMonsterMoveType::Teleport &&
		TeleportStep == ETeleportTypeState::IdleWait &&
		CachedMonster->GetMonsterState() == EMonsterState::Idle)
	{
		CurrentIdleTime += DeltaTime;
	}

	UpdateMovement();
}

APlayerBase* AMonsterAIController::FindNearestPlayer() const
{
	if (!CachedMonster)
	{
		return nullptr;
	}

	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundPlayers);

	APlayerBase* NearestPlayer = nullptr;
	float NearestDistance = DetectRadius * DetectRadius;

	for (AActor* FoundActor : FoundPlayers)
	{
		APlayerBase* Player = Cast<APlayerBase>(FoundActor);
		if (!IsValid(Player))
		{
			continue;
		}

		const float Distance = FVector::DistSquared(CachedMonster->GetActorLocation(), Player->GetActorLocation());
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestPlayer = Player;
		}
	}

	return NearestPlayer;
}

void AMonsterAIController::UpdateMovement()
{
	if (!CachedMonster)
		return;
	
	if (CachedMonster->GetMontagePlayingState())
		return;
	
	switch (MonsterMoveType)
	{
	case EMonsterMoveType::BasicMove:
		{
			MoveToTarget();
			break;
		}
	case EMonsterMoveType::Teleport:
		{
			// 텔레포트 시작과 끝으로 나누어짐, 내 생각엔 로직 자체를 나누는게 맞는듯 
			TeleportToTarget();
			break;
		}
	case EMonsterMoveType::Standing:
		break;
	case EMonsterMoveType::End:
		break;
	}
}

void AMonsterAIController::MoveToTarget()
{
	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindNearestPlayer();
	}

	if (!CurrentTarget.IsValid())
	{
	//	if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);
		StopMovement();
		return;
	}
	const float DistanceToTarget = FVector::Dist(CachedMonster->GetActorLocation(), CurrentTarget->GetActorLocation());
	
	if (DistanceToTarget >= DetectRadius)
	{
		CurrentTarget = nullptr;
	//	if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);
		StopMovement();
		return;
	}

	if (DistanceToTarget <= AttackRange)
	{
		//if (CachedMonster->GetMonsterState() != EMonsterState::Swing)
			CachedMonster->SetMonsterState(EMonsterState::Swing);	
		StopMovement();
		return;
	}
	//if (CachedMonster->GetMonsterState() != EMonsterState::Chase)
		CachedMonster->SetMonsterState(EMonsterState::Chase);

	MoveToActor(CurrentTarget.Get(), AttackRange - 50.f);
	
}

void AMonsterAIController::MoveToTargetLocation()
{
	MoveToLocation(TargetLocation);
}

void AMonsterAIController::TeleportToTarget()
{
	if (!CachedMonster)
	{
		return;
	}

	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindNearestPlayer();
	}

	if (!CurrentTarget.IsValid())
	{
		TeleportStep = ETeleportTypeState::IdleWait;
		CurrentIdleTime = 0.0f;
		if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);
		return;
	}
	
	const float DistanceToTarget = FVector::Dist(
		CachedMonster->GetActorLocation(),
		CurrentTarget->GetActorLocation()
	);

	if (DistanceToTarget > DetectRadius)
	{
		CurrentTarget = nullptr;
		TeleportStep = ETeleportTypeState::IdleWait;
		CurrentIdleTime = 0.0f;
		CachedMonster->SetMonsterState(EMonsterState::Idle);
		return;
	}

	if (TeleportStep != ETeleportTypeState::IdleWait)
	{
		return;
	}

	if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
	{
		CachedMonster->SetMonsterState(EMonsterState::Idle);
	}

	if (CurrentIdleTime < MaxIdleTime)
	{
		return;
	}
	
	TeleportStep = ETeleportTypeState::Attack;
	
	if (DistanceToTarget <= AttackRange)
	{
		CachedMonster->SetMonsterState(EMonsterState::Swing);
		bTeleport = true;
	}
	else
	{
		CachedMonster->SetMonsterState(EMonsterState::Fire);
		++FireCount;
		if (FireCount >= 2)
		{
			bTeleport = true;
			FireCount = 0;
		}
	}
}

void AMonsterAIController::NotifyAttackAnimationFinished()
{
	if (!CachedMonster)
		return;
	if (!bTeleport && TeleportStep == ETeleportTypeState::Attack)
	{
		// 만약 텔레포트 못하는데 공격상태면 Idle로 
		TeleportStep = ETeleportTypeState::IdleWait;
		CurrentIdleTime = 0.0f;
		CachedMonster->SetMonsterState(EMonsterState::Idle);
		return;
	}
	
	switch (TeleportStep)
	{
		case ETeleportTypeState::Attack:
		{
			TeleportStep = ETeleportTypeState::TeleportEnter;
			CachedMonster->SetMonsterState(EMonsterState::TeleportEnter);
			break;
		}
		case ETeleportTypeState::TeleportEnter:
		{
			TeleportStep = ETeleportTypeState::TeleportMove;
			if (CurrentTarget.IsValid())
			{
				CachedMonster->MoveTeleport(this, CurrentTarget->GetActorLocation());
			}
			TeleportStep = ETeleportTypeState::TeleportExit;
			CachedMonster->SetMonsterState(EMonsterState::TeleportExit);
			break;
		}
		case ETeleportTypeState::TeleportExit:
		{
			TeleportStep = ETeleportTypeState::IdleWait;
			CurrentIdleTime = 0.0f;
			CachedMonster->SetMonsterState(EMonsterState::Idle);
			bTeleport = false;
			break;
		}
		default:
			break;
	}
}



