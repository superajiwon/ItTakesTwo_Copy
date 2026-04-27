
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Healable.generated.h"

UINTERFACE(MinimalAPI)
class UHealable : public UInterface
{
	GENERATED_BODY()
};

class ITTAKESTWO_COPY_API IHealable
{
	GENERATED_BODY()

public:
	virtual void Heal(float HealAmount) {}
};
