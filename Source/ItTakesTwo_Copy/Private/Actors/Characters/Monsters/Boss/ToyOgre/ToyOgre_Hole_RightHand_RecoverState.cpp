// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_RightHand_RecoverState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_Hole_RightHand_RecoverState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_RightHand_Recover);
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->RightHandRecoverMontage);
}

void UToyOgre_Hole_RightHand_RecoverState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_RightHand_RecoverState::Exit()
{
}

void UToyOgre_Hole_RightHand_RecoverState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("RightHandRecoverEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->GrabBothHandsStateClass);
	}
}
