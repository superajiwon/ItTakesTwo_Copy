// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/MonsterAIController.h"
#include "Shared/ITTGameInstance.h"
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
	
	// if (CachedMonster)
	// {
	// 	DetectRadius = CachedMonster->GetDetectRadius();
	// 	AttackRange = CachedMonster->GetAttackRange();
	// 	MonsterMoveType = CachedMonster->GetMoveType();
	// 	MaxIdleTime = CachedMonster->GetMaxIdleTime();
	//
	// }
	
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

	DetectRadius = CachedMonster->GetDetectRadius();
	AttackRange = CachedMonster->GetAttackRange();
	MonsterMoveType = CachedMonster->GetMoveType();
	MaxIdleTime = CachedMonster->GetMaxIdleTime();

}
void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
		return;

	if (const UITTGameInstance* GI = GetGameInstance<UITTGameInstance>())
	{
		if (GI->IsGameplayPausedForLoading())
		{
			StopMovement();
			return;
		}
	}

	if (!CachedMonster)
	{
		CachedMonster = Cast<AMonsterBase>(GetPawn());
		if (!CachedMonster)
			return;
	}

	if (CachedMonster->IsDead())
	{
		StopMovement();
		return;
	}

	if (CurrentTarget.IsValid())
		ReTargetTime += DeltaTime;

	if (MonsterMoveType == EMonsterMoveType::Teleport &&
		TeleportStep == ETeleportTypeState::IdleWait &&
		CachedMonster->GetMonsterState() == EMonsterState::Idle)
	{
		CurrentIdleTime += DeltaTime;
	}
	else if (bRestTime)
	{
		CurrentIdleTime += DeltaTime;
	}
	UpdateMovement(DeltaTime);
}


TObjectPtr<ACharacterBase> AMonsterAIController::FindNearestPlayer() const
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

void AMonsterAIController::UpdateMovement(float DeltaTime)
{
	if (!CachedMonster)
		return;
	
	if (CachedMonster->GetMontagePlayingState())
		return;
	
	switch (MonsterMoveType)
	{
	case EMonsterMoveType::BasicMove:
		{
			MoveToTarget(DeltaTime);
			break;
		}
	case EMonsterMoveType::Teleport:
		{
			TeleportToTarget(DeltaTime);
			break;
		}
	case EMonsterMoveType::Standing:
		{
			Standing(DeltaTime);
			break;
		}
	case EMonsterMoveType::MoveForward:
		{
			MoveToForward(DeltaTime);
			break;
		}
	case EMonsterMoveType::End:
		break;
	}
}

void AMonsterAIController::MoveToTarget(float DeltaTime)
{
	bool bTargetChanged = false;

	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindNearestPlayer();
		ReTargetTime = 0.f;
		bTargetChanged = true;
	}

	if (ReTargetTime > MaxReTargetTime)
	{
		CurrentTarget = FindNearestPlayer();
		ReTargetTime = 0.f;
		bTargetChanged = true;
	}

	if (!CurrentTarget.IsValid())
	{
		if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);

		StopMovement();
		return;
	}

	if (bRestTime)
	{
		StopMovement();
		LookAtTargetSmooth(DeltaTime);

		if (CurrentIdleTime < MaxIdleTime)
			return;

		bRestTime = false;
		CurrentIdleTime = 0.f;
	}

	const float DistanceToTarget =
		FVector::Dist(CachedMonster->GetActorLocation(), CurrentTarget->GetActorLocation());

	if (DistanceToTarget >= DetectRadius)
	{
		CurrentTarget = nullptr;

		if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);

		StopMovement();
		return;
	}

	if (DistanceToTarget <= AttackRange)
	{
		if (CachedMonster->GetMonsterState() != EMonsterState::Swing)
			CachedMonster->SetMonsterState(EMonsterState::Swing);

		StopMovement();
		return;
	}

	const bool bWasChasing =
		CachedMonster->GetMonsterState() == EMonsterState::Chase;

	if (!bWasChasing)
	{
		CachedMonster->SetMonsterState(EMonsterState::Chase);
	}

	if (!bWasChasing || bTargetChanged)
	{
		const float MoveAcceptanceRadius = FMath::Max(0.f, AttackRange - 20.f);
		MoveToActor(
			CurrentTarget.Get(),
			MoveAcceptanceRadius,
			false,
			true,
			false
		);
	}
}


void AMonsterAIController::MoveToTargetLocation(float DeltaTime)
{
	MoveToLocation(TargetLocation);
}

void AMonsterAIController::TeleportToTarget(float DeltaTime)
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

void AMonsterAIController::Standing(float DeltaTime)
{
	if (!CachedMonster)
	{
		return;
	}

	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindNearestPlayer();
		if (CurrentTarget.IsValid())
		{
			ReTargetTime= 0.f;
			if (CachedMonster->GetMonsterState() != EMonsterState::Detect)
				CachedMonster->SetMonsterState(EMonsterState::Detect);
			LookAtTargetSmooth(DeltaTime);
			// SetFocalPoint(CurrentTarget->GetActorLocation());
			CachedMonster->SetDetectPlayer(true);
			return;
		}
	}
	// 못찾으면
	if (!CurrentTarget.IsValid())
	{
		CurrentIdleTime = 0.0f;
		if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);
		return;
	}
	
	//SetFocalPoint(CurrentTarget->GetActorLocation());
	LookAtTargetSmooth(DeltaTime);
	if (ReTargetTime > MaxReTargetTime)
	{
		CurrentTarget = FindNearestPlayer();
		ReTargetTime = 0.f;

		// 재탐색 후에도 타겟을 못 찾으면 Idle로 복귀
		if (!CurrentTarget.IsValid())
		{
			CurrentIdleTime = 0.0f;
			if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
				CachedMonster->SetMonsterState(EMonsterState::Idle);
			CachedMonster->SetDetectPlayer(false);
			return;
		}
	}
	
	if (bRestTime)
	{ 
		if (CurrentIdleTime < MaxIdleTime)
		{	
			StopMovement();
			return;
		}
		else
		{
			bRestTime = false;
			CurrentIdleTime = 0.f;
		}
	}
	
	const float DistanceToTarget = FVector::Dist(CachedMonster->GetActorLocation(), CurrentTarget->GetActorLocation());
	
	if (DistanceToTarget >= DetectRadius)
	{
		CurrentTarget = nullptr;
		if (CachedMonster->GetMonsterState() != EMonsterState::Idle)
			CachedMonster->SetMonsterState(EMonsterState::Idle);
		StopMovement();
		CachedMonster->SetDetectPlayer(false);
		return;
	}

	if (DistanceToTarget <= AttackRange)
	{
		if (CachedMonster->GetMonsterState() != EMonsterState::Fire)
			CachedMonster->SetMonsterState(EMonsterState::Fire);	
		StopMovement();
		return;
	}

}

void AMonsterAIController::MoveToForward(float DeltaTime)
{
	if (!CachedMonster)
	{
		return;
	}
	
	TargetLocation = CachedMonster->GetMesh()->GetComponentLocation() + 
		CachedMonster->GetActorForwardVector() * 800.f;
	MoveToLocation(TargetLocation);
	
	if (CachedMonster->GetOverlapToTarget())
	{
		if (CachedMonster->GetMonsterState() != EMonsterState::Swing)
			CachedMonster->SetMonsterState(EMonsterState::Swing);
	}
	else
	{
		if (CachedMonster->GetMonsterState() != EMonsterState::Chase)
			CachedMonster->SetMonsterState(EMonsterState::Chase);
	}
}

void AMonsterAIController::NotifyAttackAnimationFinished()
{
	if (!CachedMonster)
		return;
	if (CachedMonster->GetMoveType() == EMonsterMoveType::Teleport)
	{
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
	else
	{
		if (CachedMonster->GetMonsterState() == EMonsterState::Swing ||
			CachedMonster->GetMonsterState() == EMonsterState::Attack ||
			CachedMonster->GetMonsterState() == EMonsterState::Fire)
		{
			CachedMonster->SetMonsterState(EMonsterState::Idle);
			CurrentIdleTime = 0.0f;
			bRestTime = true;
		}
	}
}



void AMonsterAIController::LookAtTargetSmooth(float DeltaTime)
{
	if (!CachedMonster || !CurrentTarget.IsValid())
		return;

	FVector Dir = CurrentTarget->GetActorLocation() - CachedMonster->GetActorLocation();
	Dir.Z = 0.f;

	if (Dir.IsNearlyZero())
		return;

	const FRotator TargetRot = Dir.Rotation();
	const FRotator CurrentRot = CachedMonster->GetActorRotation();

	const FRotator NewRot = FMath::RInterpTo(
		CurrentRot,
		TargetRot,
		DeltaTime,
		5.f // 회전 속도. 낮을수록 느림
	);

	CachedMonster->SetActorRotation(NewRot);
}
