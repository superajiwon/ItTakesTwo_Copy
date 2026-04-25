
#include "Actors/Characters/Players/Cody/CodyAnimInstance.h"
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Shared/Components/HitBoxComponent.h"


void UCodyAnimInstance::AnimNotify_SpecialOn()
{
	Super::AnimNotify_SpecialOn();
	
}

void UCodyAnimInstance::AnimNotify_UltimateOn()
{
	Super::AnimNotify_UltimateOn();
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	Owner->UltimateCollision->CollisionOn();
}

