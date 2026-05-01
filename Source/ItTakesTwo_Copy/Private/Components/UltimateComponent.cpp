
#include "Components/UltimateComponent.h"
#include "Net/UnrealNetwork.h"

UUltimateComponent::UUltimateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	
	SetIsReplicatedByDefault(true);
}


void UUltimateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUltimateComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	
	if (bIsUltimateActive)
	{
		CurUltimateGauge -= (DecreasePerSec * DeltaTime);
		if (CurUltimateGauge <= 0.0f)
		{
			CurUltimateGauge = 0.0f;
			EndUltimate();
		}
	}
}

void UUltimateComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UUltimateComponent, CurUltimateGauge);
	DOREPLIFETIME(UUltimateComponent, bIsUltimateActive);
}

void UUltimateComponent::AddGauge(float GaugeAmount)
{	
	if (bIsUltimateActive) return;
	
	CurUltimateGauge += GaugeAmount;
	
	if (CurUltimateGauge >= MaxUltimateGauge)
		CurUltimateGauge = MaxUltimateGauge;
}

bool UUltimateComponent::CanUseUltimate()
{
	// 게이지가 100% 찼을 때만 발동 가능
	return (CurUltimateGauge >= MaxUltimateGauge) && !bIsUltimateActive;
}

void UUltimateComponent::ActivateUltimate()
{
	if (!GetOwner()->HasAuthority()) return;
	
	bIsUltimateActive = true;
}

void UUltimateComponent::EndUltimate()
{
	if (!GetOwner()->HasAuthority()) return;
	
	bIsUltimateActive = false;

	// 캐릭터쪽에 전달
	OnUltimateFinish.Broadcast();
}
