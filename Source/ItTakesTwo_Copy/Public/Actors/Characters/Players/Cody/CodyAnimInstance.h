
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
	
	
	// === Collision ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Niagara")
	TObjectPtr<UNiagaraSystem> ProjectileNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Niagara")
	TObjectPtr<UNiagaraSystem> OverlapNiagara;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	float ProjectileSpeed = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	float ProjectileLifeTime = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	float ProjectileRadius = 60.0f;

};
