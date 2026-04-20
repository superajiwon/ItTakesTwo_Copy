
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ITTPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ITTAKESTWO_COPY_API AITTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AITTPlayerController();
	
protected:
	virtual void BeginPlay() override;
	
};
