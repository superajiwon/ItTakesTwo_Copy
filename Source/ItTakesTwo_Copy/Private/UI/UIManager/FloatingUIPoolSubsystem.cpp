
#include "UI/UIManager/FloatingUIPoolSubsystem.h"

#include "UI/Actor/FloatingUIActor.h"

#define MAX_UIPOOLSIZE 30

bool UFloatingUIPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer)) return false;
	
	UWorld* World = Cast<UWorld>(Outer);  
	if (!World) return false;
	
	FString WorldName = World->GetName();
	if (WorldName.Contains(TEXT("Lv_Dungeon")) || WorldName.Contains(TEXT("Lv_CowDungeon")))
	{
		return true;
	}
	
	return false;
}

AFloatingUIActor* UFloatingUIPoolSubsystem::GetFloatingUIActor(TSubclassOf<AFloatingUIActor> ActorClass, const FVector& Location)
{
	if (!IsValid(ActorClass)) return nullptr;
	
	if (FloatingUIActorPools.IsEmpty())
	{
		InitializePool(ActorClass);
	}
	
	for (AFloatingUIActor* FloatingUIActor : FloatingUIActorPools)
	{
		if (FloatingUIActor && FloatingUIActor->IsHidden())
		{
			FloatingUIActor->SetActorLocation(Location);
			return FloatingUIActor;
		}
	}
	
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AFloatingUIActor* NewActor = World->SpawnActor<AFloatingUIActor>(ActorClass, Location, FRotator::ZeroRotator, SpawnParams);
		if (NewActor)
		{
			FloatingUIActorPools.Add(NewActor);
			return NewActor;
		}
	}
	
	return nullptr;
}

void UFloatingUIPoolSubsystem::InitializePool(TSubclassOf<AFloatingUIActor> ActorClass)
{
	UWorld* World = GetWorld();
	if (!World || !IsValid(ActorClass)) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int32 i = 0; i < MAX_UIPOOLSIZE; i++)
	{
		AFloatingUIActor* FloatingUI = World->SpawnActor<AFloatingUIActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (FloatingUI)
		{
			FloatingUI->SetActorHiddenInGame(true);
			FloatingUI->SetActorTickEnabled(false);
			
			FloatingUIActorPools.Add(FloatingUI);
		}
	}
}
