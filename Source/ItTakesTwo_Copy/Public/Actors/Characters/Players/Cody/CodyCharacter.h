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
	TObjectPtr<UHitSphereComponent> BasicCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHitBoxComponent> UltimateCollision;
	
	// === Input Actions ===
	virtual void BaseAttack(const FInputActionValue& Value) override;
	virtual void SpecialAttack(const FInputActionValue& Value) override;
	virtual void Dash(const FInputActionValue& Value) override;
	virtual void Ultimate(const FInputActionValue& Value) override;
};
