// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_HoleEnterState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Components/HitBoxComponent.h"


void UToyOgre_HoleEnterState::Enter()
{
	const FVector HoleMeshWorldLocation(-500.0f, 500.0f, 0.0f);

	OwnerOgre->StartHoleEnterAtMeshWorldLocation(
		HoleMeshWorldLocation,
		TEXT("Ogre_EnterHole"),
		false
	);

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
		// if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		// {
		// 	if (USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>())
		// 	{
		// 		SoundManager->PlaySFX2D(TEXT("Ogre_GrapBoth"));
		// 	}
		// }
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->GrabBothHandsStateClass);
	}
}
