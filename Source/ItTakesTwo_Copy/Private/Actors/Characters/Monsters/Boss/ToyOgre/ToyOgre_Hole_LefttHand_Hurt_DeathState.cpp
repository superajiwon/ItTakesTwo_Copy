// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_LefttHand_Hurt_DeathState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_Hole_LefttHand_Hurt_DeathState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_LeftHand_Hurt_Death);
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->LeftHandDeathMontage);
	OwnerOgre->DeactivateHandColliders();
	OwnerOgre->ClearHandRegenTimers();
	OwnerOgre->DeactivateHandColliders();
}

void UToyOgre_Hole_LefttHand_Hurt_DeathState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_LefttHand_Hurt_DeathState::Exit()
{
}

void UToyOgre_Hole_LefttHand_Hurt_DeathState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("LeftHandDeathEnd"))
	{
		OwnerOgre->Destroy();
//		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->LeftHandHurtDeathStateClass);
	}
}
