// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_TelegraphState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_TelegraphState::Enter()
{
	TelegraphCurrentTime = 0.f;
	OwnerOgre->SetToyOgreState(EToyOgreState::Telegraph);
}

void UToyOgre_TelegraphState::Tick(float DeltaTime)
{
	OwnerOgre->RotateToCurrentTarget(DeltaTime, 5.f);
	TelegraphCurrentTime += DeltaTime;
	if (TelegraphCurrentTime > TelegraphMaxTime)
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->ChargeStartStateClass);
	}
}

void UToyOgre_TelegraphState::Exit()
{
}
