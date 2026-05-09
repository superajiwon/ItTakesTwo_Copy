
#include "Components/SkillComponent.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/PlayerActionData.h"
#include "Components/UltimateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Components/HitSphereComponent.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 리슨 서버에서 이 컴포넌트가 복제되도록 설정
	SetIsReplicatedByDefault(true);
	
	CooldownTimes.Add(EActionType::Basic, 0.1f);
	CooldownTimes.Add(EActionType::Special, 3.0f);
	CooldownTimes.Add(EActionType::Dash, 2.0f);
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerBase* Owner = Cast<APlayerBase>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &USkillComponent::OnMontageEnded);
		}
	}
}


void USkillComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USkillComponent, CooldownMultiplier);
}

// ===
//  [진입점] 로컬 입력 핸들러에서 호출
//  리슨 서버 분기:
//    방장(HasAuthority) -> 바로 Multicast
//    조인 클라이언트		-> Server RPC -> 서버 -> Multicast 
void USkillComponent::RequestExecuteSkill(EActionType ActionType, int32 ComboStep, int32 MontageIdx)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	
	if (!CanExecuteSkill(ActionType)) return;
	
	if (ActionType != EActionType::Ultimate)
	{
		StartCooldown(ActionType);
	}
	
	if (Owner->HasAuthority())
	{	// 방장이니까 바로 호출
		Multicast_PlayerSkillEffect(ActionType, ComboStep, MontageIdx);
	}
	else
	{	// 클라이언트니까 서버에 RPC 요청
		Server_ExecuteSkill(ActionType, ComboStep, MontageIdx);
	}
}

// ===
//  [서버] 클라이언트로부터 RPC 수신
//  필요할 때 여기서 서버 전용 상태 변경 (궁극기 전환 등)
void USkillComponent::Server_ExecuteSkill_Implementation(EActionType ActionType, int32 ComboStep, int32 MontageIdx)
{
	APlayerBase* Owner = Cast<APlayerBase>(GetOwner());
	if (!Owner) return;
	
	if (ActionType == EActionType::Ultimate)
	{
		// 서버에서 이중 검증
		if (!CanExecuteSkill(ActionType)) return;
		
		if (Owner->GetUltimateComponent())
		{
			Owner->GetUltimateComponent()->ActivateUltimate();
		}
		
		Owner->OnUltimateActivated();
	}
	else
	{
		// [중요] 캐릭터별 궁극기 캔슬 처리를 CanExecuteSkill 검증보다 먼저 수행
		// CancelUltimateOnAction 내부에서 bIsSkillPlaying이 리셋될 수 있으므로
		// 반드시 먼저 호출해야 이후 CanExecuteSkill이 정확한 판단을 내릴 수 있음
		Owner->CancelUltimateOnAction(ActionType);
		
		// 서버에서 이중 검증 (CancelUltimateOnAction 이후 실행)
		if (!CanExecuteSkill(ActionType)) return;
		
		StartCooldown(ActionType);
	}
	
	Multicast_PlayerSkillEffect(ActionType, ComboStep, MontageIdx);
}

// ===
//  [전체] 서버 + 모든 클라이언트에서 실행
//  ActionData에서 몽타주를 꺼내 PlayAnimMontage() 호출
void USkillComponent::Multicast_PlayerSkillEffect_Implementation(EActionType ActionType, int32 ComboStep, int32 MontageIdx)
{
	APlayerBase* Owner = Cast<APlayerBase>(GetOwner());
	if (!Owner || !Owner->ActionData) return;
	
	//! [수정] 무기 타격 기록을 몽타주가 재생되는 정확히 이 시점에 딱 1번만 초기화합니다.
	//! 이렇게 하면 애니메이션 노티파이가 네트워크 보간으로 인해 수십 번 흔들려도 타격 기록은 공격 1회당 1번만 초기화됩니다.
	TArray<UHitBoxComponent*> HitBoxes;
	Owner->GetComponents<UHitBoxComponent>(HitBoxes);
	for (UHitBoxComponent* HitBox : HitBoxes)
	{
		HitBox->ClearHitRecords();
	}
	
	TArray<UHitSphereComponent*> HitSpheres;
	Owner->GetComponents<UHitSphereComponent>(HitSpheres);
	for (UHitSphereComponent* HitSphere : HitSpheres)
	{
		HitSphere->ClearHitRecords();
	}
	
	UPlayerActionData* Data = Owner->ActionData;
	
	// 현재 상태(Normal / Ultimate)에 맞는 데이터를 Owner에 전달
	// MayCharacter는 GetCurrentAttackData()를 override하여 bIsUltimateForm 반영
	FAttackModeData* AttackData = Owner->GetCurrentAttackData();
	
	UAnimMontage* Montage = nullptr;
	
	switch (ActionType)
	{
	case EActionType::Basic:
		if (AttackData && AttackData->BasicAttackCombos.IsValidIndex(ComboStep))
		{
			TArray<UAnimMontage*> Montages = AttackData->BasicAttackCombos[ComboStep].Montages;
			if (Montages.IsValidIndex(MontageIdx))
				Montage = Montages[MontageIdx];
		}
		break;
	case EActionType::Special:
		if (AttackData)
			Montage = AttackData->SpecialAttackMontage;
		break;
	case EActionType::Dash:
		Montage = Data->DashMontage;
		break;
	case EActionType::Ultimate:
		Montage = Data->UltimateMontage;
		break;
	case EActionType::TakeDamage:
		{
			TArray<UAnimMontage*> Montages = Data->TakeDamageData.Montages;
			if (Montages.IsValidIndex(MontageIdx))
				Montage = Montages[MontageIdx];
			break;
		}
	default: break;
	}
	
	if (Montage)
	{
		// 몽타주 재생이 실제로 성공했을 때만 상태 변경
		if (Owner->PlayAnimMontage(Montage) > 0.f)
		{
			bIsSkillPlaying = true;
			
			// 서버 및 모든 클라이언트에서 잠금 상태 동기화
			Owner->bIsActionLocked = true;
			Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
			
			
			if (ActionType == EActionType::Basic)
			{	// BaseAttack VFX: 캐릭터에게 위임 (캐릭터마다 다른 VFX 처리 가능)
				Owner->PlayBaseAttackVFX();
			}	
			else if (ActionType == EActionType::Ultimate)
			{   // Ultimate VFX: 궁극기 시작 시 루프 나이아가라 활성화
				Owner->PlayUltimateVFX();
			}
		}
	}
}

// === Cooldown ===
bool USkillComponent::IsOnCooldown(EActionType ActionType) const
{
	if (const bool* bIsOnCD = CooldownChecks.Find(ActionType))
	{
		return *bIsOnCD;
	}
	
	return false;
}

bool USkillComponent::CanExecuteSkill(EActionType ActionType) const
{
	if (ActionType == EActionType::Ultimate) return true;

	// 현재 몽타주가 재생 중이면 새로운 스킬(평타 콤보 제외) 차단
	if (bIsSkillPlaying)
	{
		// 평타는 콤보 시스템이 PlayerBase에서 관리하므로 허용하거나, 
		// 혹은 단순히 bIsSkillPlaying이 true일 때 모든 '새로운' 요청을 막음
		// 유저의 이전 요구사항(스킬 도중 다른 스킬 금지)을 반영
		if (ActionType != EActionType::Basic) return false;
	}

	if (IsOnCooldown(ActionType)) return false;

	return true;
}

void USkillComponent::StartCooldown(EActionType ActionType)
{
	float CooldownTime = 0;
	if (const float* Time = CooldownTimes.Find(ActionType))
	{
		CooldownTime = *Time;
	}
	
	if (CooldownTime <= 0.0f) return;
	
	CooldownChecks.Add(ActionType, true);
	OnCooldownChange.Broadcast(ActionType, true, CooldownTime);
	
	if (UWorld* World = GetWorld())
	{
		FTimerHandle& TimerHandle = CooldownTimers.FindOrAdd(ActionType);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &USkillComponent::EndCooldown, ActionType);
		
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, CooldownTime * CooldownMultiplier, false);	
	}
	
	if (GetOwner()->HasAuthority())
	{
		Client_StartCooldown(ActionType, CooldownTime);
	}
}

void USkillComponent::Client_StartCooldown_Implementation(EActionType ActionType, float CooldownTime)
{
	if (GetOwner()->HasAuthority()) return;
	
	if (IsOnCooldown(ActionType)) return;
	
	CooldownChecks.Add(ActionType, false);
	OnCooldownChange.Broadcast(ActionType, false, CooldownTime);
	
	if (UWorld* World = GetWorld())
	{
		FTimerHandle& TimerHandle = CooldownTimers.FindOrAdd(ActionType);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &USkillComponent::Client_EndCooldown, ActionType);
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, CooldownTime, false);
	}
}

void USkillComponent::EndCooldown(EActionType ActionType)
{
	CooldownChecks.Add(ActionType, false);
	
	float CooldownTime = 0;
	if (const float* Time = CooldownTimes.Find(ActionType))
	{
		CooldownTime = *Time;
	}
	OnCooldownChange.Broadcast(ActionType, false, CooldownTime);
	
	// 서버라면 클라이언트들에게도 쿨타임 종료 알림
	if (GetOwner()->HasAuthority())
	{
		Client_EndCooldown(ActionType);
	}
}

void USkillComponent::Client_EndCooldown_Implementation(EActionType ActionType)
{
	if (GetOwner()->HasAuthority()) return;
	
	CooldownChecks.Add(ActionType, false);
	
	float CooldownTime = 0;
	if (const float* Time = CooldownTimes.Find(ActionType))
	{
		CooldownTime = *Time;
	}
	OnCooldownChange.Broadcast(ActionType, false, CooldownTime);
}

void USkillComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
		bIsSkillPlaying = false;
}
