// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBase.h"
#include "ToyOgre_Monster.generated.h"



UENUM(BlueprintType)	
enum class EToyOgreState : uint8
{
	Idle,
	LookAtTarget,
	TelegraphEnter,
	Telegraph,
	ChargeStart,
	Charge,
	WallHit,
	Recover,
	Hole_Enter,
	Hole_GrabBothHands,
	Hole_RightHand_Hurt,
	Hole_GrabOnlyLeftHand,
	Hole_RightHand_Recover,
	Hole_RightHand_Hurt_Death, 
	Hole_LeftHand_Hurt,
	Hole_GrabOnlyRightHand,
	Hole_LeftHand_Recover,
	Hole_LeftHand_Hurt_Death,
	Dead
};

class AToyOgre_HandCollider;
class UToyOgre_StateBase;
class UToyOgre_StateMachineComponent;
UCLASS()
class ITTAKESTWO_COPY_API AToyOgre_Monster : public ABossBase
{
	GENERATED_BODY()

public:
	AToyOgre_Monster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	bool RotateToCurrentTarget(float DeltaTime, float RotateSpeed);
	void OnHandBroken(bool IsLeftHand);

private:
	void RightHandHurt() const;
	void LeftHandHurt();

	void SpawnHandColliders();
	
public:
	void ActivateLeftHandCollider();
	void DeactivateLeftHandCollider();

	void ActivateRightHandCollider();
	void DeactivateRightHandCollider();
	
	void ActivateHandColliders();
	void DeactivateHandColliders();
	void RegenHand(bool IsLeftHand);
	bool AreBothHandsBroken() const;


public:
	UFUNCTION()
	void OnRep_ToyOgreState();
	void SetToyOgreState(EToyOgreState NewState);
	
public:
	void PlayToyOgreMontage(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayToyOgreMontage(UAnimMontage* Montage);
	
	UFUNCTION(BlueprintCallable, Category="ToyOgre|AnimNotify")
	void AnimNotify_ToyOgre(FName EventName);


	
public:
	UToyOgre_StateMachineComponent* GetStateMachine() const
	{
		return StateMachine;
	}
	TWeakObjectPtr<AActor> GetCurrentTarget() const
	{
		return CurrentTarget;
	}
	
public:
	void SetCurrentTarget(AActor* NewTarget)
	{
		CurrentTarget = NewTarget;
	}


		
	// 상태 (애님 블프용)
public:
	UPROPERTY(ReplicatedUsing=OnRep_ToyOgreState, BlueprintReadOnly, Category="ToyOgre|Animation")
	EToyOgreState ToyOgreState = EToyOgreState::Idle;
	
	UPROPERTY()
	bool bEnterHole{false};
	
	// HandCollision
public:
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|Hand")
	TSubclassOf<AToyOgre_HandCollider> HandColliderClass;

	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|Hand")
	float HandRegenDelay = 3.f;

	UPROPERTY()
	TObjectPtr<AToyOgre_HandCollider> LeftHandCollider;

	UPROPERTY()
	TObjectPtr<AToyOgre_HandCollider> RightHandCollider;

	FTimerHandle LeftHandRegenTimer;
	FTimerHandle RightHandRegenTimer;

	// 몽타지
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> TelegraphEnterMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> ChargeStartMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> WallHitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> RecoverMontage;
	
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> HoleEnterMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> LeftHandHurtMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> RightHandHurtMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> LeftHandRecoverMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> RightHandRecoverMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> LeftHandDeathMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ToyOgre|Montage")
	TObjectPtr<UAnimMontage> RightHandDeathMontage;
	
	// 상태머신
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ToyOgre")
	TObjectPtr<UToyOgre_StateMachineComponent> StateMachine;

	// 타겟 찾기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> SelectTargetStateClass;
	
	// 바라보기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> LookAtTargetStateClass;

	// 발구르기 시작
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> TelegraphEnterStateClass;

	// 발구르기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> TelegraphStateClass;

	// 달리기 시작
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> ChargeStartStateClass;

	// 달리기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> ChargeStateClass;

	// 박치기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> WallHitStateClass;

	// 회복하기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> RecoverStateClass;
	
	// 빠지기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> HoleEnterStateClass;

	// 양쪽손 다 잡기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> GrabBothHandsStateClass;

	// 왼손만 잡기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> GrabOnlyLeftHandStateClass;

	// 오른손만 잡기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> GrabOnlyRightHandStateClass;

	// 왼손 회복하기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> LeftHandRecoverStateClass;

	// 오른손 회복하기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> RightHandRecoverStateClass;

	// 왼손 다치고 죽기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> LeftHandHurtDeathStateClass;

	// 오른손 다치고 죽기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> RightHandHurtDeathStateClass;

	// 왼손 다치기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> LeftHandHurtStateClass;

	// 오른손 다치기
	UPROPERTY(EditDefaultsOnly, Category="ToyOgre|State")
	TSubclassOf<UToyOgre_StateBase> RightHandHurtStateClass;

	
};
