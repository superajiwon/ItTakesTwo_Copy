// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_RecoverState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_RecoverState::Enter()
{
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->RecoverMontage);
	OwnerOgre->SetToyOgreState(EToyOgreState::Recover);
}

void UToyOgre_RecoverState::Tick(float DeltaTime)
{
}

void UToyOgre_RecoverState::Exit()
{
}

void UToyOgre_RecoverState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("RecoverEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->SelectTargetStateClass);
	}
}


void UToyOgre_RecoverState::OnRecoverEnd()
{
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->SelectTargetStateClass);
}
