

#include "Components/HPComponent.h"
#include "Net/UnrealNetwork.h"

UHPComponent::UHPComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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
}

void UHPComponent::ApplyDamage(int32 DamageAmount, AActor* Causer)
{
	// 서버에서만 판정할 수 있도록
	if (!GetOwner()->HasAuthority() || bIsDead) return;
	if (bIsInInvincible) return;
	
	CurHp = FMath::Clamp(CurHp - DamageAmount, 0.0f, MaxHp);

	// 호스트의 UI 갱신
	OnHPChanged.Broadcast(CurHp, MaxHp);
	
	if (CurHp <= 0.0f)
	{
		StopRecover();
		bIsDead = true;
		OnDeath.Broadcast();
		return;
	}

	// 자동 회복
	if (bIsPlayer)
	{
		StopRecover();
		StartRecoverDelay();
	}
	
	bIsInInvincible = true;	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(InvincibleTimer, this, &UHPComponent::EndInvincible, InvincibleTime, false);
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

void UHPComponent::Revive()
{
	if (!GetOwner()->HasAuthority()) return;
	
	CurHp = MaxHp; 
	bIsDead = false;
	bIsInInvincible = true;
	GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, this, &UHPComponent::EndInvincible, 5.0f, false);
	
	OnHPChanged.Broadcast(CurHp, MaxHp);
	OnRevive.Broadcast();
}

void UHPComponent::EndInvincible()
{
	bIsInInvincible = false;
	// UE_LOG(LogTemp, Log, TEXT("jiwon [서버] 피격 가능!"));
}

void UHPComponent::OnRep_IsDead()
{
	if (bIsDead)
	{
		OnDeath.Broadcast();
	}
	else
	{
		OnRevive.Broadcast();
	}
}

void UHPComponent::OnRep_Invincible()
{
	// todo 무적 시 이펙트?
}

void UHPComponent::StartRecoverDelay()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RecoverDelayTimer, this, &UHPComponent::StartRecover, RecoverDelayTime, false);
	}
}

void UHPComponent::StartRecover()
{
	if (CurHp >= MaxHp) return;
	
	bIsRecovering = true;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RecoverTickTimer, this, &UHPComponent::RecoverTick, RecoverSpeed, true);
	}
}

void UHPComponent::RecoverTick()
{
	if (CurHp >= MaxHp)
	{
		StopRecover();
		return;
	}
	
	ApplyHeal(RecoverAmount);
}

void UHPComponent::StopRecover()
{
	bIsRecovering = false;
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RecoverDelayTimer);
		World->GetTimerManager().ClearTimer(RecoverTickTimer);
	}
}

