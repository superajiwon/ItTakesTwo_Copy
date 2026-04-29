
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Components/HitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"

ACodyCharacter::ACodyCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/Cody_Wizard/SkeletalMeshes/Cody_Wizard.Cody_Wizard'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	
	AttackColliderPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttackColliderPoint"));
	AttackColliderPoint->SetupAttachment(GetCapsuleComponent());
	AttackColliderPoint->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	
	BaseCollision = CreateDefaultSubobject<UHitSphereComponent>(TEXT("BaseCollision"));
	BaseCollision->AttachToComponent(AttackColliderPoint, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info BasicHitCompInfo(FName("Player_CodyBasic"), FName("PlayerWeapon"), FVector(250.0f,0.0f,0.0f), 200.f);
	BaseCollision->InitializeHitComp(BasicHitCompInfo, GetTargetName());
	BaseCollision->CollisionOff();
	
	SpecialProjectilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpecialProjectilePoint"));
	
	UltimateCollision = CreateDefaultSubobject<UDotHitBoxComponent>(TEXT("UltimateCollision"));
	UltimateCollision->AttachToComponent(AttackColliderPoint, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info SwordHitCompInfo(FName("Player_CodyUltimate"), FName("PlayerWeapon"), FVector(550.0f,0.0f,88.0f), FVector(500.0f, 50.0f, 50.0f));
	UltimateCollision->InitializeHitComp(SwordHitCompInfo, GetTargetName());
	UltimateCollision->CollisionOff();
}

void ACodyCharacter::SetWeaponCollision(bool bEnable)
{
	if (!BaseCollision) return;

	if (bEnable)
	{
		// Debug
		BaseCollision->SetHiddenInGame(false);
		int32 RandDamage = FMath::RandRange(5, 18);
		BaseCollision->SetDamage(RandDamage);
		BaseCollision->CollisionOn();
	}
	else
	{
		// Debug
		BaseCollision->SetHiddenInGame(true);
		BaseCollision->CollisionOff();
	}
}

void ACodyCharacter::SpecialAttack(const FInputActionValue& Value)
{
	Super::SpecialAttack(Value);
	
	if (!ProjectileNiagara) return;
	
	UVFXObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UVFXObjectPoolSubsystem>();
	if (!PoolSubsystem) return;
	
	// const FVector SpawnLocation = 
}

void ACodyCharacter::CodyTeleport(float Distance)
{
	FVector DistLocation = GetActorLocation() + GetActorForwardVector() * Distance;

	// todo Ray 로 
	// bNoCheck = true : 철창이나 좁은 틈새를 무시하고 통과하기 위해 '체크 안 함' 설정!
	TeleportTo(DistLocation, GetActorRotation(), false, true); // 
}

void ACodyCharacter::Server_CodyTeleport_Implementation()
{
	CodyTeleport(TeleportLength);
}

void ACodyCharacter::Ultimate(const FInputActionValue& Value)
{
	Super::Ultimate(Value);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACodyCharacter::EndUltimate()
{
	Super::EndUltimate();
	
	if (UltimateCollision)
	{
		UltimateCollision->CollisionOff();
		UltimateCollision->SetHiddenInGame(true);
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


