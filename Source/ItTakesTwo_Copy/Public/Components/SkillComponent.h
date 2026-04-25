
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/ITTTypes.h"
#include "SkillComponent.generated.h"

class APlayerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITTAKESTWO_COPY_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

protected:
	virtual void BeginPlay() override;

	// 네트워크 변수 복제 규칙 정의 함수
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// === Skill Call ===
	//  캐릭터 입력 핸들러에서 호출하는 진입점
	//  랜덤 선택은 호출에서 미리 끝내고 결과 인덱스만 넘겨줌
	//  Action Type : 0=BasicAttack, 1=SpecialAttack, 2=Dash, 3=Ultimate
	//  ComboStep   : BasicAttack 콤보 단계(0~2), 나머지는 0
	//  MontageIdx  : 해당 단계에서 미리 결정된 몽타주 인덱스 
	UFUNCTION(BlueprintCallable, Category="Skill")
	void RequestExecuteSkill(EActionType ActionType, int32 ComboStep, int32 MontageIdx);
	
protected:
	// 클라이언트 → 서버 (검증 및 Multicast 트리거)
	UFUNCTION(Server, Reliable)
	void Server_ExecuteSkill(EActionType ActionType, int32 ComboStep, int32 MontageIdx);
	
	// 서버 → 전체 클라이언트 (실제 몽타주 재생)
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayerSkillEffect(EActionType ActionType, int32 ComboStep, int32 MontageIdx);
};
