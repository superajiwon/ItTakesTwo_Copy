
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ultimate")
	float MaxUltimateGauge = 100.0f;
	float CurUltimateGauge = 0.0f;
	
	bool bIsUltimateActive = false;
	bool bCanUltimate = false;
	
	void AddGauge(float GaugeAmount);
	
	// bool CanUseUltimate();
	//
	// void ActivateUltimate();
};
