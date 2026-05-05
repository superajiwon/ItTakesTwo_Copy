// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_SelectTargetState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void UToyOgre_SelectTargetState::Enter()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(OwnerOgre->GetWorld(), APlayerBase::StaticClass(), Players);

	if (Players.Num() == 0)
		return;

	static int32 TargetIndex = 0;
	AActor* NewTarget = Players[TargetIndex % Players.Num()];
	TargetIndex++;

	OwnerOgre->SetCurrentTarget(NewTarget);
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->LookAtTargetStateClass);
	

}

void UToyOgre_SelectTargetState::Tick(float DeltaTime)
{
}

void UToyOgre_SelectTargetState::Exit()
{
}
