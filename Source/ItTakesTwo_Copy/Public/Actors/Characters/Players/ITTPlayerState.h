#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Shared/ITTTypes.h" 
#include "ITTPlayerState.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AITTPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category="Role")
	EPlayerRole PlayerRole;
};
