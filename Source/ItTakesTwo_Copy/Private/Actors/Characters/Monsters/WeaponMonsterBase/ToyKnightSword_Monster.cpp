

#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyKnightSword_Monster.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"
#include "Actors/Characters/Monsters/Struct/HitComp_Info.h"


AToyKnightSword_Monster::AToyKnightSword_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("Monster"), FVector(0.f, 0.f, 45.0f), FVector(20.f, 20.f, 40.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo);
}

void AToyKnightSword_Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyKnightSword_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


