// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyArbalist_Monster.h"

#include "GameFramework/CharacterMovementComponent.h"


AToyArbalist_Monster::AToyArbalist_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Standing;	
	DetectRadius = 2000.0f;
	AttackRange = 1000.f;
	MaxIdleTime = 0.8f;
	MoveSpeed = 0.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AToyArbalist_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyArbalist_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



