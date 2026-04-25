// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"

#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"

AToyOgre_Monster::AToyOgre_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::BasicMove;
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(
		FName("Monster"), 
		FName("MonsterWeapon"), 
		FVector(110.0f, 0.f, 150.0f), 
		FVector(220.f, 150.0f, 150.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
}

void AToyOgre_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyOgre_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToyOgre_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

