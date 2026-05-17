
#include "Actors/Characters/Players/May/MayCharacter.h"
#include "Actors/Characters/Players/PlayerActionData.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/StatComponent.h"
#include "Shared/Components/DotHitSphereComponent.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Components/HitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Net/UnrealNetwork.h"


AMayCharacter::AMayCharacter()
{
	this->Tags.AddUnique(TEXT("May"));
	
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
	
	UltimateCollision = CreateDefaultSubobject<UDotHitSphereComponent>(TEXT("UltimateCollision"));
	UltimateCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Root"));
	FHitComp_Info UltimateHitCompInfo(FName("Player_MayUltimate"), FName("PlayerWeapon"), FVector(0.0f,0.0f,0.0f), 300.f);
	UltimateCollision->InitializeHitComp(UltimateHitCompInfo, GetTargetName());
	UltimateCollision->CollisionOff();
	
	AlwaysNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AlwaysNiagaraComp"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SwordNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_MayAlways.NS_MayAlways'"));
	if (SwordNiagaraAsset.Succeeded()) AlwaysNiagaraComp->SetAsset(SwordNiagaraAsset.Object);
	AlwaysNiagaraComp->SetupAttachment(RootComponent); 
	AlwaysNiagaraComp->SetAutoActivate(true);
	
	UltimateNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UltimateNiagaraComp"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UltimateNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_May_Ultimate.NS_May_Ultimate'"));
	if (UltimateNiagaraAsset.Succeeded()) UltimateNiagaraComp->SetAsset(UltimateNiagaraAsset.Object);
	UltimateNiagaraComp->SetupAttachment(UltimateCollision);
	UltimateNiagaraComp->SetAutoActivate(false);
	
	DashSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("DashSplineComp"));
	DashSplineComp->SetupAttachment(GetRootComponent());
	
	DashNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashNiagaraComp"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DashNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_Spline_Fire.NS_Spline_Fire'"));
	if (DashNiagaraAsset.Succeeded()) DashNiagaraComp->SetAsset(DashNiagaraAsset.Object);
	DashNiagaraComp->SetupAttachment(DashSplineComp);
	DashNiagaraComp->SetAutoActivate(false);
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = 1;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerArrow(TEXT("/Script/Engine.StaticMesh'/Game/Models/Characters/ArrowDecal/PlayerArrow_Mesh/StaticMeshes/SM_MayArrow.SM_MayArrow'"));
	if (PlayerArrow.Succeeded()) PlayerArrowComp->SetStaticMesh(PlayerArrow.Object);
	
	OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AMayCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// bIsUltimateForm을 모든 클라이언트에 복제
	DOREPLIFETIME(AMayCharacter, bIsUltimateForm);
	DOREPLIFETIME(AMayCharacter, OriginSpeed);
}

void AMayCharacter::SetWeaponCollision(bool bEnable)
{
	if (!SwordCollision) return;
	
	if (bEnable)
	{
		int32 RandDamage = GetStatComponent()->GetRandAttackPower();
		SwordCollision->SetDamage(RandDamage);
		SwordCollision->CollisionOn();
	}
	else
	{
		SwordCollision->CollisionOff();
	}
}

void AMayCharacter::SpecialAttack(const FInputActionValue& Value)
{
	Super::SpecialAttack(Value);
}

void AMayCharacter::PlaySpecialVFX()
{
	if (!SpecialAttackVFX) return;
	
	// 캐릭터 발 아래(내려찍는 공격) 위치에 VFX 스폰
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SpecialAttackVFX,
		GetActorLocation(),
		GetActorRotation()
	);
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
	// bIsUltimateForm = !bIsUltimateForm;
	bIsUltimateForm = true;
	
	// 서버에서도 이동속도 적용 (OnRep은 클라이언트에서만 불리므로 서버는 직접 설정)
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed * 2;
	DefaultMaxWalkSpeed = OriginSpeed * 2;
}

void AMayCharacter::OnRep_IsUltimateForm()
{
	if (bIsUltimateForm)
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginSpeed * 2;
		DefaultMaxWalkSpeed = OriginSpeed * 2;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
		DefaultMaxWalkSpeed = OriginSpeed;
	}
}

void AMayCharacter::Ultimate(const FInputActionValue& Value)
{
	Super::Ultimate(Value);
	
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed * 2;
	DefaultMaxWalkSpeed = OriginSpeed * 2;
}

void AMayCharacter::EndUltimate()
{
	Super::EndUltimate();
	
	// 서버에서 관리되는 상태 롤백 (로컬 예측을 위해 Authority 체크 제거)
	bIsUltimateForm = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	DefaultMaxWalkSpeed = OriginSpeed;
	
	if (UltimateCollision)
	{
		UltimateCollision->CollisionOff();
	}
	
	// 궁극기 종료 시 나이아가라 즉시 비활성화
	if (UltimateNiagaraComp)
	{
		UltimateNiagaraComp->DeactivateImmediate();
	}
}

void AMayCharacter::CancelUltimateOnAction(EActionType ActionType)
{
	// 캔슬 되지 않도록 
}

// SkillComponent::Multicast_PlayerSkillEffect 의 Ultimate 케이스에서 호출됨
// → 서버 + 모든 클라이언트에서 실행되므로 별도 Multicast 불필요
void AMayCharacter::PlayUltimateVFX()
{
	if (UltimateNiagaraComp)
	{
		UltimateNiagaraComp->Activate(true);
	}
}

void AMayCharacter::MayDash(FVector DashDir, float Strength, float Duration)
{
	// Spline VFX용 시작 위치 (RootMotion 적용 전 현재 위치)
	const FVector DashStart = GetActorLocation();
	
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
	
	// Spline VFX 타이머: 서버에서만 설정 (Multicast 호출권은 서버만 가짐)
	// 서버 플레이어(HasAuthority + LocallyControlled)도 이 경로로 처리됨
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(DashVFXTimer, [this, DashStart]()
		{
			Multicast_PlayDashSplineVFX(DashStart, GetActorLocation());
		}, Duration, false);
	}
}

void AMayCharacter::PlayHitSFX()
{
	HitSFXID = TEXT("May_Sword_Impact");
	Super::PlayHitSFX();
}

void AMayCharacter::Server_MayDash_Implementation()
{
	MayDash(GetActorForwardVector(), DashStrength, DashDuration);
}

void AMayCharacter::Multicast_PlayDashSplineVFX_Implementation(FVector StartPos, FVector EndPos)
{
	if (!DashSplineComp || !DashNiagaraComp) return;

	// 2-포인트 Spline 구성 (시작 → 끝)
	DashSplineComp->ClearSplinePoints();
	DashSplineComp->AddSplineWorldPoint(StartPos);
	DashSplineComp->AddSplineWorldPoint(EndPos);
	DashSplineComp->UpdateSpline();

	// Niagara 에셋이 SplineLocation DataInterface를 바인딩하면 DashSplineComp를 참조함
	DashNiagaraComp->Activate(true);
}
