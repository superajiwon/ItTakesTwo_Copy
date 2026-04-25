
#include "Actors/Characters/Players/AnimNotifies/AnimNotify_CollisionOn.h"
#include "Actors/Characters/Players/PlayerBase.h"


void UAnimNotify_CollisionOn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->SetWeaponCollision(true);
		}
	}
}
