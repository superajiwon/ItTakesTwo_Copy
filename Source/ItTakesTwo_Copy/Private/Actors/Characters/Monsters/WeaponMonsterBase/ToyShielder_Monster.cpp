// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyShielder_Monster.h"

#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"

AToyShielder_Monster::AToyShielder_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterMoveType = EMonsterMoveType::BasicMove;
	DetectRadius = 2000.0f;
	AttackRange = 100.f;
	MaxIdleTime = 0.8f;
	MoveSpeed = 300.f;
	
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("MonsterWeapon"), FVector(0.f, 0.f, 5.00001f), FVector(50.f, 70.f, 100.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
	HitBoxComponent->SetDamage(30);
	HitBoxComponent->CollisionOff();
	RightHand_WeaponMeshComponent->SetupAttachment(GetMesh(), FName(TEXT("LeftHandSocket")));
}

void AToyShielder_Monster::BeginPlay()
{
	Super::BeginPlay();
}

void AToyShielder_Monster::AnimNotify_CollisionOn()
{
	if (MonsterState == EMonsterState::Swing)
		HitBoxComponent->CollisionOn();
	Super::AnimNotify_CollisionOn();
}

void AToyShielder_Monster::AnimNotify_DeadMotionEnd()
{
	Super::AnimNotify_DeadMotionEnd();
}

void AToyShielder_Monster::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Swing && HitBoxComponent)
	{
		HitBoxComponent->CollisionOff();
		HitBoxComponent->ClearHitRecords();
	}

	Super::AnimNotify_MontageEnd();
}

void AToyShielder_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// DrawDebugBox(
	// 	GetWorld(),
	// 	HitBoxComponent->GetComponentLocation(),
	// 	HitBoxComponent->GetScaledBoxExtent(),
	// 	HitBoxComponent->GetComponentQuat(),
	// 	FColor::Red,
	// 	false,
	// 	-1,
	// 	0,
	// 	2.f
	// );
}



