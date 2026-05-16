// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damagable.h"
#include "ToyOgre_HandCollider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurHealth, float, MaxHealth);

class AToyOgre_Monster;
class AFloatingUIActor;
class USphereComponent;
class UWidgetComponent;

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
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP{100.f};
	UFUNCTION()
	void OnRep_CurrentHP();
	
	UPROPERTY(ReplicatedUsing=OnRep_bBroken)
	bool bBroken{false};
	UFUNCTION()
	void OnRep_bBroken();
	
public:
	// === Damage UI ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<AFloatingUIActor> FloatingUIClass;
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ShowDamageUI(float DamageAmount, FVector SpawnLocation, FLinearColor SpawnColor = FLinearColor::Red);
	
public:
	// === HP Bar ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* HPUIComp;
	void InitHPBar();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetHPBarVisible(bool bVisible);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHPChanged OnHPChanged;
};
