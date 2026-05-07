// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ToyOgre_HandCollider.generated.h"

class AToyOgre_Monster;
class USphereComponent;

UCLASS()
class ITTAKESTWO_COPY_API AToyOgre_HandCollider : public AActor, public IDamagable
{
	GENERATED_BODY()

public:
	AToyOgre_HandCollider();

	virtual void Damage(float DamageAmount, AActor* Causer) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitHand(AToyOgre_Monster* InOwnerOgre, bool IsLeftSide);
	void ActivateHand();
	void DeactivateHand();
	void RegenHand();

	bool IsBroken() const
	{
		return bBroken;
	}
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY()
	TObjectPtr<AToyOgre_Monster> OwnerOgre;
	
	UPROPERTY(EditAnywhere)
	bool IsLeftHand{false};
	
	UPROPERTY(EditAnywhere)
	float MaxHP{100.f};

private:
	UPROPERTY(Replicated)
	float CurrentHP{100.f};
	
	UPROPERTY(Replicated)
	bool bBroken{false};
	
};
