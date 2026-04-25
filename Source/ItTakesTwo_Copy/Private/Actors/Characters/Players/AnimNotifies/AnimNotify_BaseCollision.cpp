

#include "Actors/Characters/Players/AnimNotifies/AnimNotify_BaseCollision.h"
#include "Actors/Characters/Players/PlayerBase.h"


void UAnimNotify_BaseCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->SetWeaponCollision(true);
		}
	}
}

void UAnimNotify_BaseCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->SetWeaponCollision(false);
		}
	}
}
