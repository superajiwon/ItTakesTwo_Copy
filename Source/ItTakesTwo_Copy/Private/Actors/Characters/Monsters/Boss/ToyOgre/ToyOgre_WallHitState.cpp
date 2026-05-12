// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_WallHitState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Components/HitBoxComponent.h"

void UToyOgre_WallHitState::Enter()
{
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->WallHitMontage);
	OwnerOgre->SetToyOgreState(EToyOgreState::WallHit);
	OwnerOgre->Multicast_PlayCamShake(5);
}

void UToyOgre_WallHitState::Tick(float DeltaTime)
{

}

void UToyOgre_WallHitState::Exit()
{
	OwnerOgre->HitBoxComponent->CollisionOff();
}


void UToyOgre_WallHitState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("WallHitEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->RecoverStateClass);
		OwnerOgre->SpawnMeteor();
	}
}

void UToyOgre_WallHitState::OnWallHitEnd()
{
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->RecoverStateClass);
	OwnerOgre->SpawnMeteor();
}
