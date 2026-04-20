
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkillBase;

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
	// 캐릭터에서 호출 할 스킬 시
	UFUNCTION(BlueprintCallable, Category="Skill")
	void RequestExecuteSkill(int32 SkillIndex);
	
protected:
	// 네트워크 로직 -> 서버 단독 실행
	UFUNCTION(Server, Reliable)
	void Server_ExecuteSkill(int32 SkillIndex);
	
	// 이펙트 실행
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayerSkilEffect(int32 SkillIndex);
	
protected:
	// UPROPERTY(EditDefaultsOnly, Category="Skill")
	// TArray<TSubclassOf<USkillBase>> SkillClasses;
	//
	// UPROPERTY(Transient)
	// TArray<USkillBase*> Skills;
};
