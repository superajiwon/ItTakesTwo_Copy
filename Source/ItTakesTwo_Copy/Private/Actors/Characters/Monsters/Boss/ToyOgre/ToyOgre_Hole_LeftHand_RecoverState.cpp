// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_Hole_LeftHand_RecoverState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

void UToyOgre_Hole_LeftHand_RecoverState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::Hole_LeftHand_Recover);
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->LeftHandRecoverMontage);
	
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX3D(TEXT("Ogre_Recovery"), OwnerOgre->GetActorLocation());
		}
	}
}

void UToyOgre_Hole_LeftHand_RecoverState::Tick(float DeltaTime)
{
}

void UToyOgre_Hole_LeftHand_RecoverState::Exit()
{
}

void UToyOgre_Hole_LeftHand_RecoverState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("LeftHandRecoverEnd"))
	{
		OwnerOgre->CompleteLeftHandRecover();
	}
}
