

#include "Actors/Characters/Monsters/Boss/BossBase.h"


ABossBase::ABossBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

