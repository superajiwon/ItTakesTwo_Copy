

#include "Actors/Characters/Monsters/WeaponMonsterBase/ToyKnightSword_Monster.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"

AToyKnightSword_Monster::AToyKnightSword_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MonsterMoveType = EMonsterMoveType::BasicMove;
	DetectRadius = 2000.0f;
	AttackRange = 150.f;
	MaxIdleTime = 0.8f;
	MoveSpeed = 300.f;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(RightHand_WeaponMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(FName("Monster"), FName("MonsterWeapon"), FVector(0.f, 0.f, 45.0f), FVector(20.f, 20.f, 40.f));
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
	HitBoxComponent->SetDamage(20);
	HitBoxComponent->CollisionOff();
}

void AToyKnightSword_Monster::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AToyKnightSword_Monster::AnimNotify_CollisionOn()
{
	if (MonsterState == EMonsterState::Swing)
		HitBoxComponent->CollisionOn();
	Super::AnimNotify_CollisionOn();
}

void AToyKnightSword_Monster::AnimNotify_MontageEnd()
{
	if (MonsterState == EMonsterState::Swing && HitBoxComponent)
	{
		HitBoxComponent->CollisionOff();
		HitBoxComponent->ClearHitRecords();
	}
	Super::AnimNotify_MontageEnd();
}

void AToyKnightSword_Monster::Tick(float DeltaTime)
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


