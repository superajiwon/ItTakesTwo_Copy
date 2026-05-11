
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChangedSignature, float, CurHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReviveSignature);

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

public:
	// UI에서 바인딩 할 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHPChangedSignature OnHPChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnReviveSignature OnRevive;
	
public:
	UFUNCTION(BlueprintCallable, Category="HP")
	void Revive();
	
	UFUNCTION(BlueprintCallable, Category="HP")
	float GetCurHP() const { return CurHp; }
	UFUNCTION(BlueprintCallable, Category="HP")
	float GetMaxHP() const { return MaxHp; }
	
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsDead() const { return bIsDead; }
	
	UFUNCTION(BlueprintCallable, Category="HP")
	void SetIsPlayer(bool IsPlayer) { bIsPlayer = IsPlayer; }
	
	UFUNCTION(BlueprintCallable, Category="HP")
	void SetInvincibleTime(float Time) { InvincibleTime = Time; };
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsInInvincible() const { return bIsInInvincible; }
	UFUNCTION(BlueprintCallable, Category="HP")
	bool GetIsRecovering() const { return bIsRecovering; }
	
//! 변수
protected:   
	// === HP ===
	UPROPERTY(EditAnywhere, Replicated, Category = "HP")
	float MaxHp = 100.0f;
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CurHp, Category = "HP")
	float CurHp = 0.0f;
	UFUNCTION() // HP가 복제 될 때 클라이언트에서 호출 될 함수 (UI 갱신용)
	void OnRep_CurHP();
	
	
	// === Dead === 
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_IsDead, Category = "HP|Dead")
	bool bIsDead = false;
	UFUNCTION()
	void OnRep_IsDead();
	
	
	// === Invincible === 
	FTimerHandle InvincibleTimer;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Invincible")
	bool bIsInInvincible = false;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Invincible, Category = "HP|Invincible")
	float InvincibleTime = 0.35f;
	UFUNCTION()
	void OnRep_Invincible();
	void EndInvincible();
	
	
	// === Recover ===
	FTimerHandle RecoverDelayTimer;
	FTimerHandle RecoverTickTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP|Recovering")
	bool bIsPlayer = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	bool bIsRecovering = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float RecoverDelayTime = 3.0f;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float RecoverSpeed = 0.3f;
	UPROPERTY(EditAnywhere, Replicated, Category = "HP|Recovering")
	float RecoverAmount = 1.0f;
	
	void StartRecoverDelay();
	void StartRecover();
	void RecoverTick();
	void StopRecover();

};
