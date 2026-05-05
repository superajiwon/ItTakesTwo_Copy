// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_WallHitState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_WallHitState::Enter()
{
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->WallHitMontage);
	OwnerOgre->SetToyOgreState(EToyOgreState::WallHit);
}

void UToyOgre_WallHitState::Tick(float DeltaTime)
{

}

void UToyOgre_WallHitState::Exit()
{

}


void UToyOgre_WallHitState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("WallHitEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->RecoverStateClass);
	}
}

void UToyOgre_WallHitState::OnWallHitEnd()
{
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->RecoverStateClass);
}
