
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ITTPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	None	UMETA(DisplayName = "None"),
	May		UMETA(DisplayName = "May"),
	Cody	UMETA(DisplayName = "Cody")
};

UCLASS()
class ITTAKESTWO_COPY_API AITTPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category="Role")
	EPlayerRole PlayerRole;
};
