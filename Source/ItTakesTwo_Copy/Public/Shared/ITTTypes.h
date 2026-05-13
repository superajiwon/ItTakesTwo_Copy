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

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Basic		UMETA(DisplayName = "Basic"),
	Special		UMETA(DisplayName = "Special"),
	Dash		UMETA(DisplayName = "Dash"),
	Ultimate	UMETA(DisplayName = "Ultimate"),
	TakeDamage	UMETA(DisplayName = "TakeDamage"),
};

UENUM(BlueprintType)
enum class EPlayerSlot : uint8
{
	None	UMETA(DisplayName = "None"),
	Host	UMETA(DisplayName = "Host"),
	Client	UMETA(DisplayName = "Client")
};