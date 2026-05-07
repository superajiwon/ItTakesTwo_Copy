// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_LefttHand_HurtState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_Hole_LefttHand_HurtState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_LeftHand_Hurt);
	OwnerOgre->DeactivateLeftHandCollider();
	OwnerOgre->ActivateRightHandCollider();
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->LeftHandHurtMontage);
}

void UToyOgre_Hole_LefttHand_HurtState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_LefttHand_HurtState::Exit()
{
}

void UToyOgre_Hole_LefttHand_HurtState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("LeftHandHurtEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->GrabOnlyRightHandStateClass);
	}
}

