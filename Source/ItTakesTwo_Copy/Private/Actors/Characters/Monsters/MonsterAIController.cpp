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
	{
		return;
	}
	MoveToTargetLocation();
	
}

void AMonsterAIController::MoveToTarget()
{
	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindNearestPlayer();
	}

	if (!CurrentTarget.IsValid())
	{
		StopMovement();
		return;
	}
	const float DistanceToTarget = FVector::Dist(CachedMonster->GetActorLocation(), CurrentTarget->GetActorLocation());

	if (DistanceToTarget > DetectRadius)
	{
		CurrentTarget = nullptr;
		StopMovement();
		return;
	}

	if (DistanceToTarget <= AttackRange)
	{
		StopMovement();
		return;
	}

	MoveToActor(CurrentTarget.Get(), AttackRange);
}

void AMonsterAIController::MoveToTargetLocation()
{
	
	MoveToLocation(TargetLocation);

}

