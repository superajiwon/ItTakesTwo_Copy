// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "MonsterBase.generated.h"


class UMonsterAnimInstance;
class AMonsterAIController;
class APlayerBase;

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle,
	Detect,
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
	MoveForward,
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
	void SetDetectPlayer(bool bDetectPlayer);
	
	
public:
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
	bool GetDetectedPlayer() const
	{
		return bDetectPlayer;
	}
	bool GetOverlapToTarget() const
	{
		return bOverlapedToTarget;
	}
public:
	bool GetMontagePlayingState() const;
	
	float GetDetectRadius() const;
	float GetAttackRange() const;
	
	void MontagePlay();
	
	UFUNCTION()
	void OnRep_MonsterState();
	
	UFUNCTION(BlueprintCallable, Category = "Montage")
	virtual void AnimNotify_MontageEnd();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bDetectPlayer{false};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bOverlapedToTarget{false};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bPlayingMontage{false};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float DetectRadius{50.f};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MoveSpeed{300.0f};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float AttackRange{180.0f};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxIdleTime{1.f};
	
	
	UPROPERTY(ReplicatedUsing = OnRep_MonsterState)
	EMonsterState MonsterState{EMonsterState::Idle};
	
	UPROPERTY()
	EMonsterMoveType MonsterMoveType{EMonsterMoveType::BasicMove};
	
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;

	UPROPERTY()
	UMonsterAnimInstance* AnimInstance;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> SwingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> TeleportEnterMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> TeleportExitMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DetectMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DeadMontage;
	
	
};
