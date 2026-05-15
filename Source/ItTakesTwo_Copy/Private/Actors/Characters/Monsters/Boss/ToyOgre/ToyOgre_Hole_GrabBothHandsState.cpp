// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_GrabBothHandsState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"

void UToyOgre_Hole_GrabBothHandsState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_GrabBothHands);
	OwnerOgre->ActivateHandColliders();

	OwnerOgre->Multicast_PlayOgreSFX2D(TEXT("Ogre_GrapBoth"));
}

void UToyOgre_Hole_GrabBothHandsState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_GrabBothHandsState::Exit()
{
	
}

