
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Managers/CameraManagerActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/HPComponent.h"
#include "Components/SkillComponent.h"
#include "Components/UltimateComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "ITTGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Shared/ITTGameInstance.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"
#include "UI/InGameHPBar.h"


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
	
	HPUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HPUIComp->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetAsset(TEXT("/Game/UI/Blueprints/WBP_InGameHPBar.WBP_InGameHPBar_C"));
	if (HPWidgetAsset.Succeeded()) HPUIComp->SetWidgetClass(HPWidgetAsset.Class);
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 50.0f;
	HPUIComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetDrawSize(FVector2D(50.0f, 10.0f));
	
	GetHPComponent()->SetIsPlayer(true);
	
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
	
	GetHPComponent()->OnDeath.AddDynamic(this, &APlayerBase::OnPlayerDeath);
	GetHPComponent()->OnRevive.AddDynamic(this, &APlayerBase::OnPlayerRevive);
	
	HPUIComp->InitWidget(); 
	InitHPBar();
	
	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(GetHPUIComponent()->GetWidget()))
	{
		HPBarWidget->SetColors(FLinearColor::Green);
	}
	
	// 카메라에서 찾지 못했을 경우 직접 넣어줌
	AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManagerActor::StaticClass());
	if (ACameraManagerActor* CamManager = Cast<ACameraManagerActor>(FoundCamera))
	{
		CamManager->AddTarget(this);
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// PrintNetLog();
}

void APlayerBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	APlayerController* PC = Cast<APlayerController>(GetController());
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

void APlayerBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void APlayerBase::PrintNetLog()
{
	// Super::PrintNetLog();
	// const FString ConStr = (GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetNetMode()==ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"));
	// const FString LogStr = FString::Printf(TEXT("\n%s / 궁 : %.0f"), *ConStr, UltimateComp->CurUltimateGauge);
	// DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}

void APlayerBase::PlayHitSFX()
{
	if (USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>())
	{
		SoundManager->PlaySFX2D(HitSFXID);
		// UE_LOG(LogTemp, Warning, TEXT("플레이 되는거 맞아? 왜?안돼 : %s"), *HitSFXID.ToString());
	}
}

void APlayerBase::PlaySFX(FName SoundId)
{
	if (USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>())
	{
		SoundManager->PlaySFX2D(SoundId);
	}
}


// 현재 상태 (Normal / Ultimate)에 맞는 공격 데이터 반환
// MayCharacter가 override하여 bIsUltimateForm 상태를 반영합니다.
FAttackModeData* APlayerBase::GetCurrentAttackData()
{
	if (!ActionData) return nullptr;
	return &ActionData->NormalAttackData;
}

void APlayerBase::CancelUltimateOnAction(EActionType ActionType)
{
	if (UltimateComp && UltimateComp->bIsUltimateActive)
	{		
		// bIsSkillPlaying 먼저 리셋 → 이후 CanExecuteSkill이 새 스킬을 허용할 수 있게 됨
		if (SkillComp) SkillComp->ResetSkillState();
		
		UltimateComp->EndUltimate();
		
		// 재생 중인 궁극기 몽타주를 중단
		if (ActionData && ActionData->UltimateMontage)
		{
			StopAnimMontage(ActionData->UltimateMontage);
		}
	}
}

void APlayerBase::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	if (const UITTGameInstance* GI = GetGameInstance<UITTGameInstance>())
	{
		if (GI->IsGameplayPausedForLoading())
		{
			return;
		}
	}

	FVector2D V = Value.Get<FVector2D>(); 
	const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (bIsActionLocked)
	{
		if (UltimateComp && UltimateComp->bIsUltimateActive)
		{
			FVector InputDir = ForwardDirection * V.X + RightDirection * V.Y;
			if (!InputDir.IsNearlyZero())
			{
				FRotator TargetRotation = InputDir.Rotation();
				FRotator SmoothRot = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 12.0f);
				SetActorRotation(SmoothRot);
				
				if (GetLocalRole() == ROLE_AutonomousProxy)
				{
					Server_UpdateRotation(SmoothRot);
				}
			}
		}
		
		return;
	}
	
	AddMovementInput(ForwardDirection, V.X);
	AddMovementInput(RightDirection, V.Y);
}

void APlayerBase::Server_UpdateRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
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
	if (!bInterrupted)
	{
		ResetCombo();
		bIsActionLocked = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APlayerBase::OnPlayerDeath()
{
	bIsActionLocked = true;
	ResetCombo();
	if (SkillComp) SkillComp->ResetSkillState();
	DisableInput(Cast<APlayerController>(GetController()));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	if (HasAuthority())
	{
		Multicast_PlayDeathNiagara();
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &APlayerBase::Respawn, 3.0f, false);
	}
}

void APlayerBase::OnPlayerRevive()
{
	bIsActionLocked = false;	
	ResetCombo();
	if (SkillComp) SkillComp->ResetSkillState();
	EnableInput(Cast<APlayerController>(GetController()));
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	if (HasAuthority())
	{
		Multicast_PlayReviveNiagara();
	}
}

void APlayerBase::Respawn()
{
	if (!HasAuthority()) return;
	
	APlayerBase* OtherPlayer = nullptr;
	for (TActorIterator<APlayerBase> It(GetWorld()); It; ++It)
	{
		if (*It != this)
		{
			OtherPlayer = *It;
			break;
		}
	}
	
	if (OtherPlayer && !OtherPlayer->GetHPComponent()->GetIsDead())
	{
		FVector RespawnLocation = OtherPlayer->GetActorLocation() + FVector(150.f, 0.f, 100.f);
		FRotator RespawnRotation = OtherPlayer->GetActorRotation();

		if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		{
			FNavLocation RandLocation;
			if (NavSys->GetRandomReachablePointInRadius(OtherPlayer->GetActorLocation(), 500.0f, RandLocation))
			{
				RespawnLocation = RandLocation.Location + FVector(0, 0, 100.0f);
			}
		}

		TeleportTo(RespawnLocation, RespawnRotation);
		GetHPComponent()->Revive();
		
		// UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		// if (NavSys)
		// {
		// 	FNavLocation RandLocation;
		// 	if (NavSys->GetRandomReachablePointInRadius(OtherPlayer->GetActorLocation(), 500.0f, RandLocation))
		// 	{
		// 		TeleportTo(RandLocation.Location + FVector(0, 0, 100.0f), OtherPlayer->GetActorRotation());
		// 		GetHPComponent()->Revive();
		// 	}
		// }
	}
	else
	{
		// 둘 다 죽은 경우 
		UE_LOG(LogTemp, Warning, TEXT("둘다 쥬금. 체크 포인트 필요"));
		if (AITTGameMode* GM = Cast<AITTGameMode>(GetWorld()->GetAuthGameMode()))
		{
			FTransform RespawnPoint = GM->GetRespawnTransform(this);
			if (RespawnPoint.Equals(FTransform::Identity))
			{
				UE_LOG(LogTemp, Warning, TEXT("체크포인트 없음!"));
				RespawnPoint = FTransform(GetActorRotation(), GetActorLocation() + FVector(0, 0, 100.0f));
			}
			
			TeleportTo(RespawnPoint.GetLocation(), RespawnPoint.GetRotation().Rotator());
			GetHPComponent()->Revive();
			
			if (OtherPlayer && OtherPlayer->GetHPComponent()->GetIsDead())
			{
				GetWorld()->GetTimerManager().ClearTimer(OtherPlayer->RespawnTimer);
				FTransform OtherTransform = GM->GetRespawnTransform(OtherPlayer);
				
				if (OtherTransform.Equals(FTransform::Identity))
				{
					// 위치를 못 찾으면 내 옆에 스폰
					OtherTransform = FTransform(OtherPlayer->GetActorRotation(), RespawnPoint.GetLocation() + FVector(150.0f, 0, 0));
				}
				
				OtherPlayer->TeleportTo(OtherTransform.GetLocation(), OtherTransform.GetRotation().Rotator());
				OtherPlayer->GetHPComponent()->Revive();
			}
		}
	}
}

void APlayerBase::Multicast_PlayDeathNiagara_Implementation()
{	
	if (DeathNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathNiagara, GetActorLocation(), GetActorRotation());
	}
}

void APlayerBase::Multicast_PlayReviveNiagara_Implementation()
{
	if (ReviveNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ReviveNiagara, GetActorLocation(), GetActorRotation());
	}
}

void APlayerBase::Multicast_PlayHitVFX_Implementation(FVector ImpactPoint)
{
	if (HitNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagara, ImpactPoint, FRotator::ZeroRotator);
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
	
	// if (UltimateComp->bIsUltimateActive) UltimateComp->EndUltimate();
	CancelUltimateOnAction(EActionType::Basic);
	
	if (!SkillComp->CanExecuteSkill(EActionType::Basic)) return;
	
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
	
	// if (UltimateComp->bIsUltimateActive) UltimateComp->EndUltimate();
	CancelUltimateOnAction(EActionType::Special);
	
	if (!SkillComp->CanExecuteSkill(EActionType::Special)) return;
	
	ResetCombo();
	
	bIsActionLocked = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	SkillComp->RequestExecuteSkill(EActionType::Special, 0, 0);
}

void APlayerBase::Dash(const FInputActionValue& Value)
{
	if (!SkillComp) return;
	
	// if (UltimateComp->bIsUltimateActive) UltimateComp->EndUltimate();
	CancelUltimateOnAction(EActionType::Dash);

	if (!SkillComp->CanExecuteSkill(EActionType::Dash)) return;
	
	ResetCombo();
	
	bIsActionLocked = true;
	
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

void APlayerBase::Damage(float DamageAmount, AActor* Causer)
{
	Super::Damage(DamageAmount, Causer);
	
	// 피격 몽타주 실행 
	if (UltimateComp->bIsUltimateActive) return; // 궁을 쓰고있을 때는 데미지를 받아도 애니메이션이 캔슬 되지 않게  
	if (!SkillComp) return;
	
	if (ActionData->TakeDamageData.Montages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("jiwon Take Damage 몽타주가 비어있어요"));
		return;
	}
	
	const int32 RandomIdx = FMath::RandRange(0, ActionData->TakeDamageData.Montages.Num() - 1);
	SkillComp->RequestExecuteSkill(EActionType::TakeDamage, 0, RandomIdx);
}


