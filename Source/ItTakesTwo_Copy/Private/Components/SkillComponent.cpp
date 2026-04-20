

#include "Components/SkillComponent.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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

void USkillComponent::RequestExecuteSkill(int32 SkillIndex)
{
	// todo 서버일때는 바로 실행
	// todo 클라이언트 일때는 Server_ExecuteSkill(SkillIndex) 실행
}

void USkillComponent::Server_ExecuteSkill_Implementation(int32 SkillIndex)
{
}

void USkillComponent::Multicast_PlayerSkilEffect_Implementation(int32 SkillIndex)
{
}

