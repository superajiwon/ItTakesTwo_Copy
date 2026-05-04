
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/ITTTypes.h"
#include "SkillComponent.generated.h"

class APlayerBase;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangeSignature, EActionType, ActionType, bool, bIsOnCooldown, float, CooldownTime);

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
	
public:
	// === Cooldown ===
	UPROPERTY(BlueprintAssignable, Category="Skill|Events")
	FOnCooldownChangeSignature OnCooldownChange; // UI 바인딩용

	UPROPERTY(EditAnywhere, Category="Skill|Cooldown")
	TMap<EActionType, float> CooldownTimes;
	
	UFUNCTION(BlueprintCallable, Category="Skill")
	bool IsOnCooldown(EActionType ActionType) const;
	
	UFUNCTION(BlueprintCallable, Category="Skill")
	bool CanExecuteSkill(EActionType ActionType) const;
	
	UFUNCTION(BlueprintCallable, Category="Skill")
	bool IsSkillPlaying() const { return bIsSkillPlaying; }
	
	UFUNCTION(BlueprintCallable, Category="Skill")
	void ResetSkillState() { bIsSkillPlaying = false; }
		
protected:
	UFUNCTION(Client, Reliable)
	void Client_StartCooldown(EActionType ActionType, float CooldownTime);
	UFUNCTION(Client, Reliable)
	void Client_EndCooldown(EActionType ActionType);
	
private:
	UPROPERTY(EditAnywhere, Replicated, Category="Skill|Cooldown")
	float CooldownMultiplier = 1.0f; // 쿨타임 배속
	
	bool bIsSkillPlaying = false; // 스킬 진행 여부 (다른 스킬 몽타주 진행 중일 경우 다른 스킬 사용 불가)
	
	TMap<EActionType, FTimerHandle> CooldownTimers; // 서버 전용 타이머 
	
	TMap<EActionType, bool> CooldownChecks; // 서버/로컬용 쿨타임 상태 플래그 
	
	void StartCooldown(EActionType ActionType);
	void EndCooldown(EActionType ActionType);
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
