// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyMage_Monster.h"

#include "Actors/Characters/Monsters/MonsterAIController.h"


AToyMage_Monster::AToyMage_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Teleport;
	DetectRadius = 1500.0f;
	AttackRange = 500.f;
	MaxIdleTime = 1.f;
	TargetLocationMap.Add(0, FVector(1940.0f, 0.0f, 100.0f));
	TargetLocationMap.Add(1, FVector(720.0f, -330.0f, 100.0f));
	TargetLocationMap.Add(2, FVector(720.0f, 580.0f, 100.0f));
}

void AToyMage_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyMage_Monster::MoveTeleport(AMonsterAIController* MonsterController, FVector PlayerLocation)
{
	Super::MoveTeleport(MonsterController,PlayerLocation);

	++CurrentLocationIndex;
	if (CurrentLocationIndex > 2)
	{
		CurrentLocationIndex = 0;
	}

	FVector Location = GetActorLocation();
	if (TargetLocationMap.Contains(CurrentLocationIndex))
	{
		Location = TargetLocationMap[CurrentLocationIndex];
	}

	const FVector Dir = PlayerLocation - Location;
	const bool bSuccess = TeleportTo(Location, Dir.Rotation());

	if (bSuccess)
	{
		ForceNetUpdate(); // 중요한 변화니 빨리 갱신하라고 재촉하는 함수
	}
}

void AToyMage_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



