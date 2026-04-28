
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/ITTPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent.h"
#include "Components/UltimateComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/HPComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false; // 컨트롤러 회전에 적용 되지 않도록 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// === Setting for Collision === 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerBody"));	
	this->Tags.AddUnique(TEXT("Player"));
	SetTargetName(TEXT("Monster"));
	
	// === Component ===
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));
	UltimateComp = CreateDefaultSubobject<UUltimateComponent>(TEXT("UltimateComp"));
	
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
		
	// 기본 이동속도 기억 (이동 잠금 해제 시 복구용)
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			// 몽타주 종료 시 자동으로 호출되도록 바인딩
			AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerBase::OnMontageEnded);
		}
	}
	
	if (UltimateComp)
	{
		UltimateComp->OnUltimateFinish.AddDynamic(this, &APlayerBase::EndUltimate);
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PrintNetLog();
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

void APlayerBase::PrintNetLog()
{
	const FString ConStr = (GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetNetMode()==ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"));
	const FString LogStr = FString::Printf(TEXT("%s\nHP : %f / 궁 : %f"), *ConStr, GetHPComponent()->GetCurHP(), UltimateComp->CurUltimateGauge);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 100.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}

// 현재 상태 (Normal / Ultimate)에 맞는 공격 데이터 반환
// MayCharacter가 override하여 bIsUltimateForm 상태를 반영합니다.
FAttackModeData* APlayerBase::GetCurrentAttackData()
{
	if (!ActionData) return nullptr;
	return &ActionData->NormalAttackData;
}

void APlayerBase::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	// if (bIsActionLocked) return;

	// 이동 잠금 상태에서도 회전이 필요한 경우(Ex. 코디 궁)를 위해
	// return대신 MaxWalkSpeed를 0으로 세팅하여 AddMovementInput은 허용 (회전 방향 전달)
	if (bIsActionLocked)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	}
	
	FVector2D V = Value.Get<FVector2D>(); 
	
	const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

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

void APlayerBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// bInterrupted가 false라는 것은, 다른 스킬에 의해 끊기지 않고 정상 종료되었음을 의미
	// 즉, 이 타이밍에만 이동 제한을 풀고 콤보를 리셋해야 안전합니다.
	if (!bInterrupted)
	{
		ResetCombo();
		bIsActionLocked = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
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
		
		bIsActionLocked = true; // 이동 불가
		GetCharacterMovement()->bOrientRotationToMovement = false; 
		
		CurComboIndex = 0;
		UE_LOG(LogTemp, Log, TEXT("jiwon [Combo] 첫번째 공격임"))
	}
	else
	{
		// 콤보가 불가능 할 때 입력이 들어올 경우
		if (!bCanCombo) return;
		
		UE_LOG(LogTemp, Log, TEXT("jiwon [Combo] 콤보로 들어왔을 때"))
		
		// 콤보 구간 내 입력 시 다음 콤보 증가
		bCanCombo = false; // 이 구간에서 입력 중복 처리 방지
		CurComboIndex++; 
	}
	
	// 최대 콤보를 넘어선 경우 콤보 리셋
	if (!CurData->BasicAttackCombos.IsValidIndex(CurComboIndex) || CurData->BasicAttackCombos[CurComboIndex].Montages.IsEmpty())
	{
		ResetCombo();
		return;
	}
	
	TArray<UAnimMontage*>& Montages = CurData->BasicAttackCombos[CurComboIndex].Montages;
	const int32 RandomIdx = FMath::RandRange(0, Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::Basic, CurComboIndex, RandomIdx);
}

void APlayerBase::SpecialAttack(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	
	// 다른 공격 중이라면 갈아타기 (콤보 강제 리셋 및 상태 덮어쓰기)
	ResetCombo();
	
	bIsActionLocked = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	SkillComp->RequestExecuteSkill(EActionType::Special, 0, 0);
}


void APlayerBase::Dash(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	SkillComp->RequestExecuteSkill(EActionType::Dash, 0, 0);
}

void APlayerBase::Ultimate(const FInputActionValue& Value)
{
	if (!SkillComp || !UltimateComp || !UltimateComp->CanUseUltimate()) return;
	
	ResetCombo();

	bIsActionLocked = true; // 코디, 메이 공통으로 시전 중 일단 이동 정지
	
	UltimateComp->ActivateUltimate();
	
	CurComboIndex = 0; // 콤보 초기화 
	SkillComp->RequestExecuteSkill(EActionType::Ultimate, CurComboIndex, 0);
}

void APlayerBase::TakeDamageAction()
{
	if (!SkillComp) return;
	
	if (ActionData->TakeDamageData.Montages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("jiwon Take Damage 몽타주가 비어있어요"));
		return;
	}
	
	const int32 RandomIdx = FMath::RandRange(0, ActionData->TakeDamageData.Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::TakeDamage, 0, RandomIdx);
}
