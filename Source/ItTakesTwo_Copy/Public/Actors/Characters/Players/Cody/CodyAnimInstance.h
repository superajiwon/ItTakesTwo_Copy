
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerAnimInstance.h"
#include "CodyAnimInstance.generated.h"

class UNiagaraSystem;

UCLASS()
class ITTAKESTWO_COPY_API UCodyAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	// === Anim Notify ===
	virtual void AnimNotify_DashOn() override;
	
	virtual void AnimNotify_SpecialOn() override;
	
	virtual void AnimNotify_UltimateOn() override;
	
	
	// === Projectile ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Niagara")
	TObjectPtr<UNiagaraSystem> ProjectileNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile|Niagara")
	TObjectPtr<UNiagaraSystem> OverlapNiagara;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float ProjectileSpeed = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float ProjectileLifeTime = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float ProjectileRadius = 60.0f;
};
