// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "CodyCharacter.generated.h"

class UHitBoxComponent;
class UHitSphereComponent;
class UDotHitBoxComponent;
class UNiagaraSystem;

UCLASS()
class ITTAKESTWO_COPY_API ACodyCharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	ACodyCharacter();
		
public:
	// === Collision ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> AttackColliderPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitSphereComponent> BaseCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDotHitBoxComponent> UltimateCollision;
	
	
	// === Collision ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack_Niagara")
	TObjectPtr<UNiagaraSystem> ProjectileNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack_Niagara")
	TObjectPtr<UNiagaraSystem> OverlapNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack_Niagara")
	USceneComponent* SpecialProjectilePoint;
	
	
	// === Base Attack ===
	virtual void SetWeaponCollision(bool bEnable) override;
	
	
	// === Special Attack === 
	// todo
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	
	
	// === Ultimate Attack === 
	// todo
	virtual void Ultimate(const FInputActionValue& Value) override;
	virtual void EndUltimate() override;
	
	
	// === Dash ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float TeleportLength = 300.0f;
	
	void CodyTeleport(float Distance);
	
	UFUNCTION(Server, Reliable)
	void Server_CodyTeleport();
};
