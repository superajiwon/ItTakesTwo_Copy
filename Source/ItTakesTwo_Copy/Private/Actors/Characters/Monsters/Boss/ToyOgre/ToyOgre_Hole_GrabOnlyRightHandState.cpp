// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_GrabOnlyRightHandState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"

void UToyOgre_Hole_GrabOnlyRightHandState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_GrabOnlyRightHand);
	OwnerOgre->ActivateRightHandCollider();
	OwnerOgre->DeactivateLeftHandCollider();
}

void UToyOgre_Hole_GrabOnlyRightHandState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_GrabOnlyRightHandState::Exit()
{
}
