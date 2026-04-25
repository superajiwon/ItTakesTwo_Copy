
#include "Actors/Characters/Players/AnimNotifies/AnimNotify_CanCombo.h"
#include "Actors/Characters/Players/PlayerBase.h"


void UAnimNotify_CanCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->bCanCombo = true;
		}
	}
}

void UAnimNotify_CanCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (APlayerBase* Player = Cast<APlayerBase>(MeshComp->GetOwner()))
		{
			Player->bCanCombo = false;
		}
	}
}
