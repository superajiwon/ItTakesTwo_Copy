// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/WeaponMonsterBase/AnimNotifyState_MageTeleportMove.h"

void UAnimNotifyState_MageTeleportMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp)
		return;

	StartLocation = MeshComp->GetRelativeLocation();
	ElapsedTime = 0.f;
	Duration = FMath::Max(TotalDuration, KINDA_SMALL_NUMBER);

	if (MoveMode == EMageTeleportMoveMode::Enter)
	{
		TargetLocation = StartLocation + FVector(0.f, 0.f, OffsetZ);
	}
	else
	{
		TargetLocation = StartLocation - FVector(0.f, 0.f, OffsetZ);
	}
	
	
}

void UAnimNotifyState_MageTeleportMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!MeshComp)
		return;

	ElapsedTime += FrameDeltaTime;

	const float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.f);

	const FVector NewLocation = FMath::Lerp(
		StartLocation,
		TargetLocation,
		Alpha
	);

	MeshComp->SetRelativeLocation(
		NewLocation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}

void UAnimNotifyState_MageTeleportMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
		return;

	MeshComp->SetRelativeLocation(
		TargetLocation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}
