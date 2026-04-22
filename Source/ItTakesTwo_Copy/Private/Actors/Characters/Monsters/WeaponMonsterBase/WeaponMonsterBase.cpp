
#include "Actors/Characters/Monsters/WeaponMonsterBase/WeaponMonsterBase.h"


AWeaponMonsterBase::AWeaponMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RightHand_WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponMeshComponent"));
	RightHand_WeaponMeshComponent->SetupAttachment(GetMesh(), FName(TEXT("RightHandSocket")));
}

void AWeaponMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


