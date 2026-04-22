// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "MonsterBase.generated.h"


class APlayerBase;

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle,
	Chase,
	Attack,
	Hit,
	Dead,
	End
};

class USkeletalMesh;
UCLASS()
class ITTAKESTWO_COPY_API AMonsterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void SetMonsterState(EMonsterState NewState);

	float GetDetectRadius() const;
	float GetAttackRange() const;
	UFUNCTION()
	void OnRep_MonsterState();
	
	
protected:
	UPROPERTY()
	float DetectRadius{50.f};
	
	UPROPERTY()
	float MoveSpeed{300.0f};
	
	UPROPERTY()
	float AttackRange{180.0f};
	
	UPROPERTY(ReplicatedUsing = OnRep_MonsterState)
	EMonsterState MonsterState{EMonsterState::Idle};
	
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;

	
};
