// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

class APlayerBase;
class AMonsterBase;

UCLASS()
class ITTAKESTWO_COPY_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	APlayerBase* FindNearestPlayer() const;
	void UpdateMovement();
	void MoveToTarget();
	void MoveToTargetLocation();
private:
	UPROPERTY()
	TObjectPtr<AMonsterBase> CachedMonster;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 180.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FVector TargetLocation{};
};
