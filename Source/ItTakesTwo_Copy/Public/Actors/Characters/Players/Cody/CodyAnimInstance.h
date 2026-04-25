
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerAnimInstance.h"
#include "CodyAnimInstance.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UCodyAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	// === Anim Notify ===
	virtual void AnimNotify_CollisionOn() override {};
	virtual void AnimNotify_CollisionOff() override {};
	
	UFUNCTION()
	void AnimNotify_AttackOn();
};
