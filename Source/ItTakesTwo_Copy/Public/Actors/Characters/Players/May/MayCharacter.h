// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "MayCharacter.generated.h"

class UHitBoxComponent;
class UHitSphereComponent;
class UDotHitSphereComponent;

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
	
	// === Dash ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float DashStrength = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float DashDuration = 0.25f;
	
	void MayDash(FVector DashDir, float Strength, float Duration);
	
	UFUNCTION(Server, Reliable)
	void Server_MayDash();
};
