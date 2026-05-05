// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_ChargeState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_ChargeState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Charge);
}

void UToyOgre_ChargeState::Tick(float DeltaTime)
{

	const FVector MoveDelta =
		OwnerOgre->GetActorForwardVector() * RushSpeed * DeltaTime;

	FHitResult Hit;
	OwnerOgre->AddActorWorldOffset(MoveDelta, true, &Hit);

	if (Hit.bBlockingHit)
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->WallHitStateClass);
		return;
	}

	
}

void UToyOgre_ChargeState::Exit()
{
}
