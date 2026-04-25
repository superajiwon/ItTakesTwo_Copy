
#include "Actors/Characters/Players/AnimNotifies/AnimNotify_ResetCombo.h"
#include "Actors/Characters/Players/PlayerBase.h"


void UAnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->ResetCombo();
		}
	}
}
