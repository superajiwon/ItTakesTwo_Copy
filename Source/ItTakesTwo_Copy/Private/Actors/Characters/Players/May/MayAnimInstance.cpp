
#include "Actors/Characters/Players/May/MayAnimInstance.h"

#include "Actors/Characters/Monsters/HitBoxComponent.h"
#include "Actors/Characters/Players/May/MayCharacter.h"


void UMayAnimInstance::AnimNotify_CollisionOn()
{
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->SwordCollision->CollisionOn();
}

void UMayAnimInstance::AnimNotify_CollisionOff()
{
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->SwordCollision->CollisionOff();
}

