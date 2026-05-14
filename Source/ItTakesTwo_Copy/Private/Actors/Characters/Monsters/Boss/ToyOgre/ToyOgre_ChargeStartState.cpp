// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_ChargeStartState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

void UToyOgre_ChargeStartState::Enter()
{
	OwnerOgre->PlayToyOgreMontage(OwnerOgre->ChargeStartMontage);
	OwnerOgre->SetToyOgreState(EToyOgreState::ChargeStart);
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager = GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX2D(TEXT("Ogre_Breath"));
		}
	}
}

void UToyOgre_ChargeStartState::Tick(float DeltaTime)
{
}

void UToyOgre_ChargeStartState::Exit()
{
}

void UToyOgre_ChargeStartState::HandleAnimNotify(FName NotifyName)
{
	if (NotifyName == TEXT("ChargeStart"))
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->ChargeStateClass);
	}
}

void UToyOgre_ChargeStartState::OnChargeStartEnd()
{
	OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->ChargeStateClass);
}
