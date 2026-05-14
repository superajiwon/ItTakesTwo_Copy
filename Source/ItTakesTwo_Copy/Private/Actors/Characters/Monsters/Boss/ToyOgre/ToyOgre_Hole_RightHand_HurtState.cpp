// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_RightHand_HurtState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

void UToyOgre_Hole_RightHand_HurtState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_RightHand_Hurt);
	OwnerOgre->DeactivateRightHandCollider();
	OwnerOgre->ActivateLeftHandCollider();
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->RightHandHurtMontage);
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX3D(TEXT("Ogre_HandHurt"), OwnerOgre->GetActorLocation());
		}
	}
}

void UToyOgre_Hole_RightHand_HurtState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_RightHand_HurtState::Exit()
{
}

void UToyOgre_Hole_RightHand_HurtState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("RightHandHurtEnd"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->GrabOnlyLeftHandStateClass);
	}
}
