// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_TelegraphEnterState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_TelegraphEnterState::Enter()
{
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->TelegraphEnterMontage);
	OwnerOgre->SetToyOgreState(EToyOgreState::TelegraphEnter);
}

void UToyOgre_TelegraphEnterState::Tick(float DeltaTime)
{
	OwnerOgre->RotateToCurrentTarget(DeltaTime, 5.f);
}

void UToyOgre_TelegraphEnterState::Exit()
{
}

void UToyOgre_TelegraphEnterState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("TelegraphEnter"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->TelegraphStateClass);
	}
}

void UToyOgre_TelegraphEnterState::OnTelegraphEnterEnd()
{
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->TelegraphStateClass);
}
