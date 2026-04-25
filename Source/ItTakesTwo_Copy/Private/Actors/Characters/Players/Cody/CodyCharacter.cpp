
#include "Actors/Characters/Players/Cody/CodyCharacter.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"
#include "Actors/Characters/Monsters/Struct/HitComp_Info.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Shared/Components/HitSphereComponent.h"

ACodyCharacter::ACodyCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/Cody_Wizard/SkeletalMeshes/Cody_Wizard.Cody_Wizard'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	
	BasicCollision = CreateDefaultSubobject<UHitSphereComponent>(TEXT("BasicCollision"));
	BasicCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Root"));
	FHitComp_Info BasicHitCompInfo(FName("Player_CodyBasic"), FName("PlayerWeapon"), FVector(250.0f,0.0f,0.0f), 200.f);
	BasicCollision->InitializeHitComp(BasicHitCompInfo);
	BasicCollision->CollisionOff();
	
	UltimateCollision = CreateDefaultSubobject<UHitBoxComponent>(TEXT("SwordCollision"));
	UltimateCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Root"));
	FHitComp_Info SwordHitCompInfo(FName("Player_CodyUltimate"), FName("PlayerWeapon"), FVector(0.0f,0.0f,0.0f), FVector( 500.0f, 50.0f, 50.0f));
	UltimateCollision->InitializeHitComp(SwordHitCompInfo);
	UltimateCollision->CollisionOff();
}

void ACodyCharacter::BaseAttack(const FInputActionValue& Value)
{
	Super::BaseAttack(Value);
	
}

void ACodyCharacter::SpecialAttack(const FInputActionValue& Value)
{
	Super::SpecialAttack(Value);
}

void ACodyCharacter::Dash(const FInputActionValue& Value)
{
	Super::Dash(Value);
}

void ACodyCharacter::Ultimate(const FInputActionValue& Value)
{
	Super::Ultimate(Value);
}

