
#include "Actors/Characters/Players/PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AActor* OwningActor = GetOwningActor();
	if (!OwningActor) return;
	
	MovementSpeed = OwningActor->GetVelocity().Size2D();
	bIsMoving = (MovementSpeed > 3.f);	
}

void UPlayerAnimInstance::AnimNotify_CanCombo()
{
	
}

void UPlayerAnimInstance::AnimNotify_SpecialOn()
{
	
}

void UPlayerAnimInstance::AnimNotify_UltimateOn()
{
	
}

