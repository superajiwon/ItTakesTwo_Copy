
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ResetCombo.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UAnimNotify_ResetCombo : public UAnimNotify
{
	GENERATED_BODY()	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};