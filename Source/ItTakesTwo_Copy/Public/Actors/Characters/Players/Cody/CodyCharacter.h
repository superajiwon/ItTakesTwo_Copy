// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "CodyCharacter.generated.h"

class UHitBoxComponent;
class UHitSphereComponent;

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
	TObjectPtr<UHitBoxComponent> UltimateCollision;
	
	
	// === Base Attack ===
	virtual void SetWeaponCollision(bool bEnable) override;
	
	
	// === Special Attack === 
	// todo
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	
	
	// === Ultimate Attack === 
	// todo
	virtual void Ultimate(const FInputActionValue& Value) override;
	
	
	// === Dash ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|State")
	float TeleportLength = 300.0f;
	
	void CodyTeleport(float Distance);
	
	UFUNCTION(Server, Reliable)
	void Server_CodyTeleport();
};
