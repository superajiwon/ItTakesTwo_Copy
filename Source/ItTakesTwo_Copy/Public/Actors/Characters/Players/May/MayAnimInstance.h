
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerAnimInstance.h"
#include "MayAnimInstance.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UMayAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	// === Anim Notify === 
	virtual void AnimNotify_CollisionOn() override ;
	virtual void AnimNotify_CollisionOff() override ;
};
