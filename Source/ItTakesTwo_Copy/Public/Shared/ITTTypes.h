#pragma once

#include "CoreMinimal.h"
#include "ITTTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	None	UMETA(DisplayName = "None"),
	May		UMETA(DisplayName = "May (Fire Knight)"),
	Cody	UMETA(DisplayName = "Cody (Ice Mage)")
};
