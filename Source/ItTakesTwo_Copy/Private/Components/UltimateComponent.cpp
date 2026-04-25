

#include "Components/UltimateComponent.h"

UUltimateComponent::UUltimateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurUltimateGauge = MaxUltimateGauge;
}


void UUltimateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UUltimateComponent::AddGauge(float GaugeAmount)
{	
	CurUltimateGauge += GaugeAmount;
	
	if (CurUltimateGauge >= MaxUltimateGauge)
		CurUltimateGauge = MaxUltimateGauge;
}
