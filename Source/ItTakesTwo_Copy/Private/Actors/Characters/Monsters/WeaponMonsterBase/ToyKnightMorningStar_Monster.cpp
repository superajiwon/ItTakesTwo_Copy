// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyKnightMorningStar_Monster.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"


AToyKnightMorningStar_Monster::AToyKnightMorningStar_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
		
	MonsterMoveType = EMonsterMoveType::BasicMove;
	DetectRadius = 5000.0f;
	AttackRange = 100.f;
	MaxIdleTime = 0.3f;
	MoveSpeed = 300.f;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("Monster"), FVector(0.f, 0.f, 45.0f), FVector(20.f, 20.f, 40.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
}

void AToyKnightMorningStar_Monster::BeginPlay()
{
	Super::BeginPlay();
	

}

void AToyKnightMorningStar_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


