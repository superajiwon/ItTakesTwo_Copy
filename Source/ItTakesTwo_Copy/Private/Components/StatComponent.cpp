

#include "Components/StatComponent.h"
#include "Net/UnrealNetwork.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 리슨 서버에서 이 컴포넌트가 복제되도록 설정
	SetIsReplicatedByDefault(true);
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 변수 네트워크 복제
	DOREPLIFETIME(UStatComponent, MoveSpeed);
	DOREPLIFETIME(UStatComponent, MinAttackPower);
	DOREPLIFETIME(UStatComponent, AttackPower);
}

