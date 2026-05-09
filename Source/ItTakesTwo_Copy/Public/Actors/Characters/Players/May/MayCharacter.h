// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "MayCharacter.generated.h"

class UHitBoxComponent;
class UHitSphereComponent;
class UDotHitSphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USplineComponent;

UCLASS()
class ITTAKESTWO_COPY_API AMayCharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AMayCharacter();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// === Weapon ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SwordComp;
	
	
	// === Collision ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitBoxComponent> SwordCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitSphereComponent> SpecialCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDotHitSphereComponent> UltimateCollision;

	
	// === Base Attack ===
	virtual void SetWeaponCollision(bool bEnable) override;
	
	
	// === Special Attack === 
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	// AnimNotify에서 ColliderOn 시 호출
	void PlaySpecialVFX();
	
	
	// === Ultimate Attack === 
	// 궁극기 폼 체인지 상태
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat|State")
	bool bIsUltimateForm = false;

	// bIsUltimateForm == false → NormalAttackData    (3콤보, 스페셜 A)
	// bIsUltimateForm == true  → UltimateAttackData  (1콤보, 스페셜 B)
	virtual FAttackModeData* GetCurrentAttackData() override;

	// bIsUltimateForm을 서버에서 토글 → Replicated로 클라이언트 동기화
	virtual void OnUltimateActivated() override;
	
	virtual void Ultimate(const FInputActionValue& Value) override;
	virtual void EndUltimate() override;
	virtual void CancelUltimateOnAction(EActionType ActionType) override;
	
	// SkillComponent Multicast에서 Ultimate 재생 시 호출
	virtual void PlayUltimateVFX() override;
	
	
	// === Dash ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float DashStrength = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float DashDuration = 0.25f;
	
	void MayDash(FVector DashDir, float Strength, float Duration);
	
	UFUNCTION(Server, Reliable)
	void Server_MayDash();
	
private:
	FTimerHandle DashVFXTimer; // Dash VFX용 타이머 (멤버로 유지해야 소멸 방지)

	
public:
	// === Niagara VFX ===
	// 몸에 항상 붙어있는 트레일 나이아가라 (공격 시에만 활성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara|Component")
	TObjectPtr<UNiagaraComponent> AlwaysNiagaraComp;
	
	// 궁극기 지속 나이아가라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara|Component")
	TObjectPtr<UNiagaraComponent> UltimateNiagaraComp;

	// Dash 궤적 Spline
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara|Component")
	TObjectPtr<USplineComponent> DashSplineComp;

	// Dash Spline을 따라 재생하는 나이아가라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara|Component")
	TObjectPtr<UNiagaraComponent> DashNiagaraComp;

	// SpecialAttack 충격 VFX 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Niagara")
	TObjectPtr<UNiagaraSystem> SpecialAttackVFX;

	// Multicast: Dash 궤적 VFX (서버에서 시작/끝 위치 수집 후 전파)
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayDashSplineVFX(FVector StartPos, FVector EndPos);
};
