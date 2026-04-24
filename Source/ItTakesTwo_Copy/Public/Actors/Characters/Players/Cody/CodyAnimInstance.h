
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
	UFUNCTION()
	virtual void AnimNotify_CollisionOn();
	UFUNCTION()
	virtual void AnimNotify_CollisionOff();
	
	UFUNCTION()
	void AnimNotify_AttackOn();
};
