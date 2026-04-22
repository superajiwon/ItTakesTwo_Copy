// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyCrusher_Monster.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"
#include "Actors/Characters/Monsters/Struct/HitComp_Info.h"


AToyCrusher_Monster::AToyCrusher_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	// HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("Monster"), FVector(0.f, 0.f, 45.0f), FVector(20.f, 20.f, 40.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
}

void AToyCrusher_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyCrusher_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToyCrusher_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

