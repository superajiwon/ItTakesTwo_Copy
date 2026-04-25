// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyCrusher_Monster.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"
#include "Actors/Characters/Monsters/Struct/HitComp_Info.h"


AToyCrusher_Monster::AToyCrusher_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MoveSpeed = 300.f;
	MonsterMoveType = EMonsterMoveType::MoveForward;
	
	BoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	BoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->SetBoxExtent(FVector(130.f, 700.f, 570.f));
	BoxComponent->SetRelativeLocation(FVector::ZeroVector);
	BoxComponent->SetCollisionProfileName("MonsterWeapon");
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AToyCrusher_Monster::OnHitBoxBeginOverlap);

}

void AToyCrusher_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyCrusher_Monster::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	bOverlapedToTarget = true;
	
}

void AToyCrusher_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



