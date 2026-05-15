// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "CodyCharacter.generated.h"

class UHitBoxComponent;
class UHitSphereComponent;
class ACodyUltimateBox;
class UNiagaraSystem;
class UNiagaraComponent;
class UChildActorComponent;

UCLASS()
class ITTAKESTWO_COPY_API ACodyCharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	ACodyCharacter();
	
protected:
	virtual void BeginPlay() override;	
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// === Collision ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> AttackColliderPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitSphereComponent> BaseCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Component")
	TObjectPtr<USceneComponent> SpecialProjectilePoint;
	
	UPROPERTY(ReplicatedUsing=OnRep_UltimateCollision, VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<ACodyUltimateBox> UltimateCollision;
	UFUNCTION()
	void OnRep_UltimateCollision();

	// === Base Attack ===
	virtual void SetWeaponCollision(bool bEnable) override;
	virtual void PlayBaseAttackVFX() override { Multicast_PlayBaseAttackVFX(); }
	
	// === Special Attack === 
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	
	
	// === Ultimate Attack === 
	virtual void Ultimate(const FInputActionValue& Value) override;
	virtual void EndUltimate() override;
	
	virtual void CancelUltimateOnAction(EActionType ActionType) override;
	
	
	// === Dash ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float TeleportLength = 500.0f;
	
	void CodyTeleport(float Distance);
	
	UFUNCTION(Server, Reliable)
	void Server_CodyTeleport();
	
	
	// === Niagara VFX ===
	// 손에 항상 켜져있는 Infinite 나이아가라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara|Component")
	TObjectPtr<UNiagaraComponent> HandNiagaraComp;

	// BaseAttack 시 재생할 나이아가라 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Niagara")
	TObjectPtr<UNiagaraSystem> BaseAttackVFX;

	// Teleport 도착 지점에 재생할 나이아가라 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Niagara")
	TObjectPtr<UNiagaraSystem> TeleportVFX;

	// 모든 클라이언트에서 BaseAttack VFX 재생
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayBaseAttackVFX();

	// 모든 클라이언트에서 Teleport 도착 VFX 재생
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayTeleportVFX(FVector Location);
};
