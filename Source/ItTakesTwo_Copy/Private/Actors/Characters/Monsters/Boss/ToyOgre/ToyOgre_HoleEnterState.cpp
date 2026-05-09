// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_HoleEnterState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Components/HitBoxComponent.h"

void UToyOgre_HoleEnterState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_Enter);
	// OwnerOgre->SetActorLocation(FVector(-500.0f, 500.0f, 0.0f));
	OwnerOgre->GetMesh()->SetWorldLocation(FVector(-500.0f,500.0f,0.0f));
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->HoleEnterMontage);
	OwnerOgre->bEnterHole = true;
	if (OwnerOgre->HitBoxComponent)
	{
		OwnerOgre->HitBoxComponent->CollisionOff();
		OwnerOgre->HitBoxComponent->ClearHitRecords();
	}
}


void UToyOgre_HoleEnterState::Tick(float DeltaTime)
{
}

void UToyOgre_HoleEnterState::Exit()
{
}

void UToyOgre_HoleEnterState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("HoleEnterEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->GrabBothHandsStateClass);
	}
}
