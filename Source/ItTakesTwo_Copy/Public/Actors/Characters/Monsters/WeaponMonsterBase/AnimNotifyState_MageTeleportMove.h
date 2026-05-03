// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MageTeleportMove.generated.h"

UENUM(BlueprintType)
enum class EMageTeleportMoveMode : uint8
{
	Enter,
	Exit
};

UCLASS()
class ITTAKESTWO_COPY_API UAnimNotifyState_MageTeleportMove : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Teleport")
	EMageTeleportMoveMode MoveMode = EMageTeleportMoveMode::Enter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Teleport")
	float OffsetZ = -300.f;

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyTick(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
	
	
private:
	FVector StartLocation;
	FVector TargetLocation;
	float ElapsedTime = 0.f;
	float Duration = 0.f;
};
