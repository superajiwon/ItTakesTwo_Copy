
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Healable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealable : public UInterface
{
	GENERATED_BODY()
};

class ITTAKESTWO_COPY_API IHealable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void Heal(float Amount);
};
