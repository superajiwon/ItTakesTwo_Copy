// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_GrabOnlyLeftHandState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"

void UToyOgre_Hole_GrabOnlyLeftHandState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_GrabOnlyLeftHand);
	OwnerOgre->ActivateLeftHandCollider();
	OwnerOgre->DeactivateRightHandCollider();
}

void UToyOgre_Hole_GrabOnlyLeftHandState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_GrabOnlyLeftHandState::Exit()
{
	
}
