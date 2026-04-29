
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChangedSignature, float, CurHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITTAKESTWO_COPY_API UHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHPComponent();

protected:
	virtual void BeginPlay() override;
	
	// 네트워크 변수 복제 규칙 정의 함수
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:	
	UFUNCTION(BlueprintCallable, Category="HP")
	void ApplyDamage(int32 DamageAmount, AActor* Causer);
	UFUNCTION(BlueprintCallable, Category="HP")
	void ApplyHeal(float HealAmount);

private:
	// HP가 복제 될 때 클라이언트에서 호출 될 함수 (UI 갱신용)
	UFUNCTION()
	void OnRep_CurHP();
	
public:
	// UI에서 바인딩 할 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHPChangedSignature OnHPChanged;
	
public:
	UFUNCTION(BlueprintCallable, Category="HP")
	float GetCurHP() const { return CurHp; }
	UFUNCTION(BlueprintCallable, Category="HP")
	float GetMaxHP() const { return MaxHp; }
	
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsDead() const { return bIsDead; }
	
	UFUNCTION(BlueprintCallable, Category="HP")
	void SetInvincibleTime(float Time) { InvincibleTime = Time; };
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsInInvincible() const { return bIsInInvincible; }
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsRecovering() const { return bIsRecovering; }
	
//! 변수
protected:   
	// HP
	UPROPERTY(EditAnywhere, Replicated, Category = "HP")
	float MaxHp = 100.0f;
	// ReplicatedUsing을 통해 값이 복제 될 때 OnRep_CurHP 함수 실행
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CurHp, Category = "HP")
	float CurHp = 0.0f;
	
	UPROPERTY(EditAnywhere, Replicated, Category = "HP")
	bool bIsDead = false;
	
	// 무적
	FTimerHandle InvincibleTimer;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Invincible")
	bool bIsInInvincible = false;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Invincible, Category = "HP|Invincible")
	float InvincibleTime = 0.5f;
	UFUNCTION()
	void OnRep_Invincible();
	void EndInvincible();

	
	// 자동 회복
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	bool bIsRecovering = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float MaxRecoverTime = 0.0f;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float CurRecoverTime = 0.0f;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float RecoverAmount = 0.0f;
	

};
