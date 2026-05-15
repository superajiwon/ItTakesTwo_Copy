// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_RightHand_Hurt_DeathState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

void UToyOgre_Hole_RightHand_Hurt_DeathState::Enter()
{
	
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_RightHand_Hurt_Death);
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->RightHandDeathMontage);
	OwnerOgre->DeactivateHandColliders();
	OwnerOgre->ClearHandRegenTimers();
	OwnerOgre->DeactivateHandColliders();
	
	// if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	// {
	// 	if (USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>())
	// 	{
	// 		// SoundManager->PlaySFX3D(TEXT("Ogre_Death"), OwnerOgre->GetActorLocation());
	// 		// SoundManager->PlaySFX2D(TEXT("Ogre_Death"));
	// 	
	// 	}
	// }
	OwnerOgre->Multicast_PlayOgreSFX2D(TEXT("Ogre_Death"));
	OwnerOgre->PlayDeathSequence();
}

void UToyOgre_Hole_RightHand_Hurt_DeathState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_RightHand_Hurt_DeathState::Exit()
{
}

void UToyOgre_Hole_RightHand_Hurt_DeathState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("RightHandDeathEnd"))
	{
		OwnerOgre->Destroy();
		//OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->RightHandHurtDeathStateClass);
	}
}
