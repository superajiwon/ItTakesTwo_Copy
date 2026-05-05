// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_LookAtTargetState.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"

void UToyOgre_LookAtTargetState::Enter()
{
	OwnerOgre->SetToyOgreState(EToyOgreState::LookAtTarget);
}

void UToyOgre_LookAtTargetState::Tick(float DeltaTime)
{
	TWeakObjectPtr<AActor> Target = OwnerOgre->GetCurrentTarget();
	if (!Target.IsValid())
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->SelectTargetStateClass);
		return;
	}
	AActor* TargetActor = Target.Get();
	const FVector ToTarget = TargetActor->GetActorLocation() - OwnerOgre->GetActorLocation();
	const FRotator TargetRot = FRotationMatrix::MakeFromX(ToTarget).Rotator();

	FRotator NewRot = FMath::RInterpTo(
		OwnerOgre->GetActorRotation(),
		FRotator(0.f, TargetRot.Yaw, 0.f),
		DeltaTime,
		5.f
	);

	OwnerOgre->SetActorRotation(NewRot);

	if (FMath::Abs(FMath::FindDeltaAngleDegrees(NewRot.Yaw, TargetRot.Yaw)) < 3.f)
	{
		OwnerOgre->GetStateMachine()->ChangeState(OwnerOgre->TelegraphEnterStateClass);
	}
}

void UToyOgre_LookAtTargetState::Exit()
{
}
