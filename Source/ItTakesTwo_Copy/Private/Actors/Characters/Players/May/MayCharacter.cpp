
#include "Actors/Characters/Players/May/MayCharacter.h"
#include "Actors/Characters/Players/PlayerActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "Net/UnrealNetwork.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Components/HitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"

AMayCharacter::AMayCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/May_Castle/SkeletalMeshes/May_Castle.May_Castle'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	
	SwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	SwordComp->SetupAttachment(GetMesh(), FName(TEXT("RightAttachSocket")));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/Characters/May_Castle/May_Sword/StaticMeshes/May_Sword.May_Sword'"));
	if (TempSwordMesh.Succeeded()) SwordComp->SetStaticMesh(TempSwordMesh.Object);
	
	SwordCollision = CreateDefaultSubobject<UHitBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->AttachToComponent(SwordComp, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info SwordHitCompInfo(FName("Player_MaySword"), FName("PlayerWeapon"), FVector(0.0f,-5.679186f,119.102255f), FVector( 10.0f, 40.0f, 90.0f));
	SwordCollision->InitializeHitComp(SwordHitCompInfo, GetTargetName());
	SwordCollision->CollisionOff();
	
	SpecialCollision = CreateDefaultSubobject<UHitSphereComponent>(TEXT("SpecialCollision"));
	SpecialCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Root"));
	FHitComp_Info SpecialHitCompInfo(FName("Player_MaySpecial"), FName("PlayerWeapon"), FVector(0.0f,0.0f,0.0f), 500.f);
	SpecialCollision->InitializeHitComp(SpecialHitCompInfo, GetTargetName());
	SpecialCollision->CollisionOff();
	
	UltimateCollision = CreateDefaultSubobject<UHitSphereComponent>(TEXT("UltimateCollision"));
	UltimateCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Root"));
	FHitComp_Info UltimateHitCompInfo(FName("Player_MayUltimate"), FName("PlayerWeapon"), FVector(0.0f,0.0f,0.0f), 300.f);
	UltimateCollision->InitializeHitComp(UltimateHitCompInfo, GetTargetName());
	UltimateCollision->CollisionOff();
}

void AMayCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// bIsUltimateForm을 모든 클라이언트에 복제
	DOREPLIFETIME(AMayCharacter, bIsUltimateForm);
}

void AMayCharacter::SetWeaponCollision(bool bEnable)
{
	if (!SwordCollision) return;
	
	if (bEnable)
	{
		// Debug
		SwordCollision->SetHiddenInGame(false);
		SwordCollision->CollisionOn();
	}
	else
	{
		// Debug
		SwordCollision->SetHiddenInGame(true);
		SwordCollision->CollisionOff();
	}
}

void AMayCharacter::SpecialAttack(const FInputActionValue& Value)
{
	Super::SpecialAttack(Value);
}

FAttackModeData* AMayCharacter::GetCurrentAttackData()
{
	if (!ActionData) return nullptr;
	
	if (bIsUltimateForm && ActionData->UltimateAttackData.BasicAttackCombos.Num() > 0)
		return &ActionData->UltimateAttackData;
	
	return &ActionData->NormalAttackData;
}

// Server_ExecuteSkill_Implementation에서 서버 단독 호출
void AMayCharacter::OnUltimateActivated()
{
	// 서버에서만 호출되므로 HasAuthority() 체크 불필요
	bIsUltimateForm = !bIsUltimateForm;
}

void AMayCharacter::Ultimate(const FInputActionValue& Value)
{
	Super::Ultimate(Value);
}

void AMayCharacter::MayDash(FVector DashDir, float Strength, float Duration)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
		ConstantForce->InstanceName = TEXT("MayDash");
		ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Override;
		ConstantForce->Priority = 5;
		ConstantForce->Force = DashDir * Strength;
		ConstantForce->Duration = Duration;
		
		ConstantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
		ConstantForce->FinishVelocityParams.ClampVelocity = 600.0f;
		
		MoveComp->ApplyRootMotionSource(ConstantForce);
	}
}

void AMayCharacter::Server_MayDash_Implementation()
{
	MayDash(GetActorForwardVector(), DashStrength, DashDuration);
}
