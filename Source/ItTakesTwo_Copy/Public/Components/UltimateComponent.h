
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UltimateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITTAKESTWO_COPY_API UUltimateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUltimateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
