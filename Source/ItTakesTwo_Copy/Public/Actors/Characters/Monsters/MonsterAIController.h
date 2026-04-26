// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterBase.h"
#include "MonsterAIController.generated.h"

class APlayerBase;
class AMonsterBase;

UENUM()
enum class ETeleportTypeState : uint8
{
	IdleWait,
	Attack,
	TeleportEnter,
	TeleportMove,
	TeleportExit
};

UCLASS()
class ITTAKESTWO_COPY_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController();
	TObjectPtr<ACharacterBase> FindNearestPlayer() const;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	
	void UpdateMovement();

	void MoveToTarget();
	void MoveToTargetLocation();
	void TeleportToTarget();
	void Standing();
	void MoveToForward();
	
public:
	void NotifyAttackAnimationFinished();
	
private:
	UPROPERTY()
	TObjectPtr<AMonsterBase> CachedMonster;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY()
	EMonsterMoveType MonsterMoveType{EMonsterMoveType::BasicMove};
	
	UPROPERTY(EditAnywhere, Category = "MonsterAI")
	float DetectRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "MonsterAI")
	float AttackRange = 180.0f;

	UPROPERTY(EditAnywhere, Category = "MonsterAI")
	FVector TargetLocation{};

private:
	bool bTeleport{false};
	bool bWaitingAttackEnd{false};
	bool bRestTime{false};

	
	int32 FireCount{0};

	
	float CurrentIdleTime{0.0f};
	float MaxIdleTime{1.0f};
	float ReTargetTime{0.0f};
	float MaxReTargetTime{3.0f};
	ETeleportTypeState TeleportStep = ETeleportTypeState::IdleWait;
};
