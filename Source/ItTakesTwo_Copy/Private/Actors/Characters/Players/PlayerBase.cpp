
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/ITTPlayerController.h"

#include "Components/SkillComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerBase::APlayerBase()
{
	bUseControllerRotationYaw = false; // 컨트롤러 회전에 적용 되지 않도록 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
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

// ===
//  기본 공격 (LMB)
//  랜덤 인덱스를 로컬에서 미리 결정 → SkillComp에 전달 
//  SkillComp가 RPC를 통해 모든 클라이언트에 동기화
//  Cody (Montages 3개): RandRange(0,2) → 실제 랜덤
//  May  (Montages 1개): RandRange(0,0) = 0 → 항상 동일 → 동일 코드로 처리 가능
void APlayerBase::BaseAttack(const FInputActionValue& Value)
{
	if (!SkillComp || !ActionData) return;
	
	UE_LOG(LogTemp, Warning, TEXT("왜요왜요왜?"));
	
	FAttackModeData* CurData = GetCurrentAttackData();
	if (!CurData) return;
	
	// 재생하려는 콤보 인덱스가 있는지 확인
	if (!CurData->BasicAttackCombos.IsValidIndex(CurComboIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("재생하려는 콤보 인덱스가 있는지 확인: %d"), CurComboIndex);
		return;
	}
	
	TArray<UAnimMontage*>& Montages = CurData->BasicAttackCombos[CurComboIndex].Montages;
	if (Montages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("몽타주가 비어있어요: %d"), CurComboIndex);
		return;
	}
	
	// 로컬에서 랜덤 결정 → 결과 인덱스를 RPC에 실어 전송
	// 이렇게 해야 서버/클라이언트 모두 동일한 몽타주를 재생할 수 있음
	const int32 RandomIdx = FMath::RandRange(0, Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::Basic, CurComboIndex, RandomIdx);

	UE_LOG(LogTemp, Warning, TEXT("CurComboIndex: %d / RandomIdx: %d"), CurComboIndex, RandomIdx);
	
	// 다음 콤보 단계로 진행
	// 최대 콤보를 넘기면 0으로 리셋
	// (실제 리셋 타이밍은 AnimNotify "ComboReset"에서 CurrentComboIndex = 0 으로 처리 권장)
	// 콤보 인덱스는 소유 클라이언트/방장 로컬에서 관리
	CurComboIndex++;
	if (CurComboIndex >= CurData->BasicAttackCombos.Num())
		CurComboIndex = 0;
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
	SkillComp->RequestExecuteSkill(EActionType::Ultimate, 0, 0);
}
