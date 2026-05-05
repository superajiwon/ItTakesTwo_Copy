// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateBase.h"


UToyOgre_StateMachineComponent::UToyOgre_StateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UToyOgre_StateMachineComponent::Init(AToyOgre_Monster* InOwner)
{
	OwnerOgre = InOwner;
}

void UToyOgre_StateMachineComponent::ChangeState(TSubclassOf<UToyOgre_StateBase> NewStateClass)
{
	if (!NewStateClass || !OwnerOgre)
		return;

	if (CurrentState)
	{
		CurrentState->Exit();
	}

	CurrentState = NewObject<UToyOgre_StateBase>(this, NewStateClass);
	CurrentState->Init(OwnerOgre);
	CurrentState->Enter();
}

void UToyOgre_StateMachineComponent::TickState(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->Tick(DeltaTime);
	}
}

void UToyOgre_StateMachineComponent::HandleAnimNotify(FName NotifyName)
{	
	if (CurrentState)
	{	
		CurrentState->HandleAnimNotify(NotifyName);
	}
}
	

