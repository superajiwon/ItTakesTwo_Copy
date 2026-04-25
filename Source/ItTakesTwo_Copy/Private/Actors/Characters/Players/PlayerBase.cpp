
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/ITTPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerBase::APlayerBase()
{
	bUseControllerRotationYaw = false; // 컨트롤러 회전에 적용 되지 않도록 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// === Setting for Collision === 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerBody"));	
	this->Tags.AddUnique(TEXT("Player"));
	SetTargetName(TEXT("Monster"));
	
	// === Component ===
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));
	
	// === Input ===
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_PlayerMapping.IMC_PlayerMapping'"));
	if (TempIMC.Succeeded()) IMC_PlayerMapping = TempIMC.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded()) IA_Move = TempMoveInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempBaseInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_BaseAttack.IA_BaseAttack'"));
	if (TempBaseInput.Succeeded()) IA_BaseAttack = TempBaseInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempSpecInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SpecialAttack.IA_SpecialAttack'"));
	if (TempSpecInput.Succeeded()) IA_SpecialAttack = TempSpecInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempDashInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Dash.IA_Dash'"));
	if (TempDashInput.Succeeded()) IA_Dash = TempDashInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempUltiInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Ultimate.IA_Ultimate'"));
	if (TempUltiInput.Succeeded()) IA_Ultimate = TempUltiInput.Object;
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APlayerBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto* PC = Cast<AITTPlayerController>(GetController());
	if (PC && PC->IsLocalPlayerController())
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem) Subsystem->AddMappingContext(IMC_PlayerMapping, 0);
		
		auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (PlayerInput)
		{
			PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerBase::Move);
			PlayerInput->BindAction(IA_BaseAttack, ETriggerEvent::Started, this, &APlayerBase::BaseAttack);
			PlayerInput->BindAction(IA_SpecialAttack, ETriggerEvent::Started, this, &APlayerBase::SpecialAttack);
			PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &APlayerBase::Dash);
			PlayerInput->BindAction(IA_Ultimate, ETriggerEvent::Started, this, &APlayerBase::Ultimate);  
		}
	}
}

// ============================================================
//  현재 상태 (Normal / Ultimate)에 맞는 공격 데이터 반환
//  MayCharacter가 override하여 bIsUltimateForm 상태를 반영합니다.
// ============================================================
FAttackModeData* APlayerBase::GetCurrentAttackData()
{
	if (!ActionData) return nullptr;
	return &ActionData->NormalAttackData;
}


void APlayerBase::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D V = Value.Get<FVector2D>(); // X: 좌우, Y: 상하 (일반적인 IA_Move 설정 기준)

	const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 카메라 기준 정면과 오른쪽 방향 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, V.X);
	AddMovementInput(RightDirection, V.Y);
}

void APlayerBase::ResetCombo()
{
	bIsAttacking = false;
	bCanCombo = false;
	CurComboIndex = 0;
}

// ===
//  기본 공격 (LMB)
//  랜덤 인덱스를 로컬에서 미리 결정 → SkillComp에 전달 
//  SkillComp가 RPC를 통해 모든 클라이언트에 동기화
//  Cody (Montages 3개): RandRange(0,2) → 실제 랜덤
//  May  (Montages 1개): RandRange(0,0) = 0 → 항상 동일 → 동일 코드로 처리 가능
void APlayerBase::BaseAttack(const FInputActionValue& Value)
{
	if (!SkillComp || !ActionData) return;
	
	FAttackModeData* CurData = GetCurrentAttackData();
	if (!CurData) return;
	
	if (!bIsAttacking)
	{
		// 공격 중이 아닐 때 첫번째 공격 시작
		bIsAttacking = true;
		CurComboIndex = 0;
	}
	else
	{
		// 콤보가 불가능 할 때 입력이 들어올 경우
		if (!bCanCombo) return;
		
		// 콤보 구간 내 입력 시 다음 콤보 증가
		bCanCombo = false; // 이 구간에서 입력 중복 처리 방지
		CurComboIndex++; 
	}
	
	// 최대 콤보를 넘어선 경우 콤보 리셋
	if (!CurData->BasicAttackCombos.IsValidIndex(CurComboIndex))
	{
		ResetCombo();
		return;
	}
	
	TArray<UAnimMontage*> Montages = CurData->BasicAttackCombos[CurComboIndex].Montages;
	int32 RanIndex = FMath::RandRange(0, Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::Basic, CurComboIndex, RanIndex);
}

void APlayerBase::SpecialAttack(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	SkillComp->RequestExecuteSkill(EActionType::Special, 0, 0);
}


void APlayerBase::Dash(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	SkillComp->RequestExecuteSkill(EActionType::Dash, 0, 0);
}

void APlayerBase::Ultimate(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	CurComboIndex = 0; // 콤보 초기화 
	SkillComp->RequestExecuteSkill(EActionType::Ultimate, CurComboIndex, 0);
}

void APlayerBase::TakeDamageAction()
{
	if (!SkillComp) return;
	
	if (ActionData->TakeDamageData.Montages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Take Damage 몽타주가 비어있어요"));
		return;
	}
	
	const int32 RandomIdx = FMath::RandRange(0, ActionData->TakeDamageData.Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::TakeDamage, 0, RandomIdx);
}
