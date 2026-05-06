
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent.h"
#include "Components/StatComponent.h"
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
	SpecialProjectilePoint->SetupAttachment(AttackColliderPoint);
	SpecialProjectilePoint->SetRelativeLocation(FVector(70.0f,0.0f,125.0f));
	
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
	
}

void ACodyCharacter::CodyTeleport(float Distance)
{
	FVector StartLocation = GetActorLocation();
	FVector DistLocation = StartLocation + (GetActorForwardVector() * Distance);
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bCanTeleport = false; 
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), DistLocation, ECC_Visibility, Params);
	if (bHit && Hit.GetActor())
	{
		bCanTeleport = Hit.GetActor()->ActorHasTag(FName("CanTeleport"));
	}
	
	SetActorLocation(DistLocation, !bCanTeleport); // bSweep
	
	// bNoCheck = true : 철창이나 좁은 틈새를 무시하고 통과하기 위해 '체크 안 함' 설정! 
	// TeleportTo는 막혀있어도 그걸 뚫고 지나가기 때문에 사용하면 안된다 
	// TeleportTo(DistLocation, GetActorRotation(), false, bCanTeleport); 
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
		UltimateCollision->SetHiddenInGame(true);
		UltimateCollision->CollisionOff();
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


