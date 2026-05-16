
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FloatingUIPoolSubsystem.generated.h"

class AFloatingUIActor;

UCLASS()
class ITTAKESTWO_COPY_API UFloatingUIPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	AFloatingUIActor* GetFloatingUIActor(TSubclassOf<AFloatingUIActor> ActorClass, const FVector& Location);
	
private:
	void InitializePool(TSubclassOf<AFloatingUIActor> ActorClass);
	
private:
	UPROPERTY()
	TArray<AFloatingUIActor*> FloatingUIActorPools;
};
