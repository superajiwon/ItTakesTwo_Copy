#pragma once

#include "CoreMinimal.h"
#include "Shared/ITTTypes.h"
#include "HUDPlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FHUDPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category="HUD|Player")
	EPlayerRole Role{EPlayerRole::None};

	UPROPERTY(BlueprintReadOnly, Category="HUD|Player")
	EPlayerSlot Slot{EPlayerSlot::None};

	UPROPERTY(BlueprintReadOnly, Category="HUD|HP")
	float CurHP{0.f};

	UPROPERTY(BlueprintReadOnly, Category="HUD|HP")
	float MaxHP{0.f};

public:
	float GetHPPercent() const
	{
		return MaxHP > 0.f ? CurHP / MaxHP : 0.f;
	}
};