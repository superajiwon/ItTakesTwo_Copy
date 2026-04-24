// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyShielder_Monster.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"


AToyShielder_Monster::AToyShielder_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::BasicMove;
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("Monster"), FVector(0.f, 0.f, 5.00001f), FVector(50.f, 70.f, 100.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
	
}

void AToyShielder_Monster::BeginPlay()
{
	Super::BeginPlay();
}

void AToyShielder_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



