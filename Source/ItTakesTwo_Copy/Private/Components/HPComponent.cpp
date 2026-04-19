

#include "Components/HPComponent.h"
#include "Net/UnrealNetwork.h"

UHPComponent::UHPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 리슨 서버에서 이 컴포넌트가 복제되도록 설정
	SetIsReplicatedByDefault(true);
}


void UHPComponent::BeginPlay()
{
	Super::BeginPlay();

	// 서버에서 초기 HP를 MAXHP로 초기화 (클라이언트는 무조건 서버로부터 값을 전달받아야함)
	if (GetOwner()->HasAuthority())
	{
		CurHp = MaxHp;
	}
}

void UHPComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 변수 네트워크 복제
	DOREPLIFETIME(UHPComponent, CurHp);
	DOREPLIFETIME(UHPComponent, MaxHp);
	DOREPLIFETIME(UHPComponent, bIsDead);
	DOREPLIFETIME(UHPComponent, bIsInInvincible);
	DOREPLIFETIME(UHPComponent, InvincibleTime);
	DOREPLIFETIME(UHPComponent, CurInvincibleTime);
	DOREPLIFETIME(UHPComponent, bIsRecovering);
	DOREPLIFETIME(UHPComponent, MaxRecoverTime);
	DOREPLIFETIME(UHPComponent, CurRecoverTime);
	DOREPLIFETIME(UHPComponent, RecoverAmount);
}

void UHPComponent::ApplyDamage(float DamageAmount)
{
	// 서버에서만 판정할 수 있도록
	if (!GetOwner()->HasAuthority() || bIsDead || bIsInInvincible) return;
	
	CurHp = FMath::Clamp(CurHp - DamageAmount, 0.0f, MaxHp);
	
	// 호스트의 UI 갱신
	OnHPChanged.Broadcast(CurHp, MaxHp);
	
	if (CurHp <= 0.0f)
	{
		bIsDead = true;
		
		// todo 사망했을 때 
	}
}

void UHPComponent::ApplyHeal(float HealAmount)
{
	// 서버에서만 판정할 수 있도록
	if (!GetOwner()->HasAuthority() || bIsDead) return;
	
	CurHp = FMath::Clamp(CurHp + HealAmount, 0.0f, MaxHp);
	
	// 호스트의 UI 갱신
	OnHPChanged.Broadcast(CurHp, MaxHp);
}

// 클라이언트에서 서버로부터 CurHP 값이 전달되었을 때 호출되는 함수
void UHPComponent::OnRep_CurHP()
{
	// todo UI Delegate 호출해 화면 갱신
	
	// EX.
	OnHPChanged.Broadcast(CurHp, MaxHp);
}


