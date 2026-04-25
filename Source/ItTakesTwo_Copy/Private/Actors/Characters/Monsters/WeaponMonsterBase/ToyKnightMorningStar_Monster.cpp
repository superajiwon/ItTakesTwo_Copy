// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyKnightMorningStar_Monster.h"

#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"

AToyKnightMorningStar_Monster::AToyKnightMorningStar_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
		
	MonsterMoveType = EMonsterMoveType::BasicMove;
	DetectRadius = 2000.0f;
	AttackRange = 100.f;
	MaxIdleTime = 0.8f;
	MoveSpeed = 300.f;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("MonsterWeapon"), FVector(0.f, 0.f, 45.0f), FVector(20.f, 20.f, 40.f));
	// HitBoxComponent->InitializeHitComp(HitCompInfo);
	// [나지원이 테스트로 추가함 헤헤..] 	
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
}

void AToyKnightMorningStar_Monster::BeginPlay()
{
	Super::BeginPlay();
	

}

void AToyKnightMorningStar_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugBox(
		GetWorld(),
		HitBoxComponent->GetComponentLocation(),
		HitBoxComponent->GetScaledBoxExtent(),
		HitBoxComponent->GetComponentQuat(),
		FColor::Red,
		false,
		-1,
		0,
		2.f
	);
}


