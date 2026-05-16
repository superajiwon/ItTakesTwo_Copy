#include "Actors/Characters/Monsters/WeaponMonsterBase/AnimNotifyState_MageTeleportMove.h"

void UAnimNotifyState_MageTeleportMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
		return;

	StartLocation = MeshComp->GetRelativeLocation();
	TargetLocation = StartLocation;

	ElapsedTime = 0.f;
	Duration = FMath::Max(TotalDuration, KINDA_SMALL_NUMBER);

	if (MoveMode == EMageTeleportMoveMode::Enter)
	{
		TargetLocation.Z = HiddenMeshZ;   // 무조건 땅속
	}
	else
	{
		TargetLocation.Z = VisibleMeshZ;  // 무조건 원래 보이는 위치
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

	FVector NewLocation = StartLocation;

	NewLocation.Z = FMath::Lerp(
		StartLocation.Z,
		TargetLocation.Z,
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

	FVector FinalLocation = MeshComp->GetRelativeLocation();

	if (MoveMode == EMageTeleportMoveMode::Enter)
	{
		FinalLocation.Z = HiddenMeshZ;
	}
	else
	{
		FinalLocation.Z = VisibleMeshZ;
	}

	MeshComp->SetRelativeLocation(
		FinalLocation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}
