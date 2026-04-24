// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "MonsterBase.generated.h"


class AMonsterAIController;
class APlayerBase;

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle,
	Chase,
	TeleportEnter,
	TeleportExit,
	Attack,
	Swing,
	Fire,
	Hit,
	Dead,
	End
};


UENUM(BlueprintType)
enum class EMonsterMoveType : uint8
{
	BasicMove,
	Teleport,
	Standing,
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
	virtual void MoveTeleport(AMonsterAIController* MonsterController, FVector PlayerLocation){};
	
	
public:
	void SetMonsterState(EMonsterState NewState);
	EMonsterState GetMonsterState() const
	{
		return MonsterState;	
	}
	EMonsterMoveType GetMoveType() const
	{
		return MonsterMoveType;
	}
	float GetMaxIdleTime() const
	{
		return MaxIdleTime;
	}
	
public:
	bool GetMontagePlayingState() const;
	float GetDetectRadius() const;
	float GetAttackRange() const;
	
	void MontagePlay();
	
	UFUNCTION()
	void OnRep_MonsterState();
	
	UFUNCTION(BlueprintCallable, Category = "Montage")
	void AnimNotify_MontageEnd();
	
protected:
	UPROPERTY()
	bool bPlayingMontage{false};
	
	UPROPERTY()
	float DetectRadius{50.f};
	
	UPROPERTY()
	float MoveSpeed{300.0f};
	
	UPROPERTY()
	float AttackRange{180.0f};
	
	UPROPERTY()
	float MaxIdleTime{1.f};
	
	
	UPROPERTY(ReplicatedUsing = OnRep_MonsterState)
	EMonsterState MonsterState{EMonsterState::Idle};
	
	UPROPERTY()
	EMonsterMoveType MonsterMoveType{EMonsterMoveType::BasicMove};
	
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY()
	UAnimInstance* AnimInstance;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> SwingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> TeleportEnterMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> TeleportExitMontage;
	
	
};
