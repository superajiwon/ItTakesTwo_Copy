
#include "Actors/Characters/Players/May/MayAnimInstance.h"

#include "Actors/Characters/Players/May/MayCharacter.h"
#include "Shared/Components/HitSphereComponent.h"


void UMayAnimInstance::AnimNotify_SpecialOn()
{
	Super::AnimNotify_SpecialOn();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->SpecialCollision->CollisionOn();
}

void UMayAnimInstance::AnimNotify_SpecialOff()
{
	Super::AnimNotify_SpecialOff();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->SpecialCollision->CollisionOff();
}

void UMayAnimInstance::AnimNotify_UltimateOn()
{
	Super::AnimNotify_UltimateOn();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->UltimateCollision->CollisionOn();
}

