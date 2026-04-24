// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyArbalist_Monster.h"


AToyArbalist_Monster::AToyArbalist_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::Standing;	
	
}

void AToyArbalist_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyArbalist_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



