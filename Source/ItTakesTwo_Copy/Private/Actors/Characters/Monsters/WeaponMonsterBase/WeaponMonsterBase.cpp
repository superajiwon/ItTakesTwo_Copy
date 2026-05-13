
#include "Actors/Characters/Monsters/WeaponMonsterBase/WeaponMonsterBase.h"

#include "GameFramework/CharacterMovementComponent.h"


AWeaponMonsterBase::AWeaponMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(TEXT("SectionCull"));
	Tags.AddUnique(TEXT("SectionCullCollision"));
	Tags.AddUnique(TEXT("SectionCullTick"));
	RightHand_WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponMeshComponent"));
	RightHand_WeaponMeshComponent->SetupAttachment(GetMesh(), FName(TEXT("RightHandSocket")));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerArrow(TEXT("/Script/Engine.StaticMesh'/Game/Models/Characters/ArrowDecal/PlayerArrow_Mesh/StaticMeshes/SM_MonsterArrow.SM_MonsterArrow'"));
	if (PlayerArrow.Succeeded()) PlayerArrowComp->SetStaticMesh(PlayerArrow.Object);
	
	HPBarColor = FLinearColor::Red;
}

void AWeaponMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AWeaponMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


