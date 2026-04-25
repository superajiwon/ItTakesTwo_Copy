
#include "Components/SkillComponent.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/PlayerActionData.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 리슨 서버에서 이 컴포넌트가 복제되도록 설정
	SetIsReplicatedByDefault(true);
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USkillComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
	
	if (Owner->HasAuthority())
	{
		// 방장이니까 바로 호출
		Multicast_PlayerSkillEffect(ActionType, ComboStep, MontageIdx);
	}
	else
	{
		// 클라이언트니까 서버에 RPC 요청
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
		Owner->OnUltimateActivated();
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
		Owner->PlayAnimMontage(Montage);
}
