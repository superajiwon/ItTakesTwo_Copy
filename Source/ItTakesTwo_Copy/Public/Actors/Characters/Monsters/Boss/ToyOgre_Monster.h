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
	Dead
};


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

	// 동작
public:
	bool RotateToCurrentTarget(float DeltaTime, float RotateSpeed);
	
	
	// 상태 (애님 블프용)
public:
	UPROPERTY(ReplicatedUsing=OnRep_ToyOgreState, BlueprintReadOnly, Category="ToyOgre|Animation")
	EToyOgreState ToyOgreState = EToyOgreState::Idle;
	
	UFUNCTION()
	void OnRep_ToyOgreState();
	void SetToyOgreState(EToyOgreState NewState);
	
public:
	void PlayToyOgreMontage(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayToyOgreMontage(UAnimMontage* Montage);
	
	UFUNCTION(BlueprintCallable, Category="ToyOgre|AnimNotify")
	void AnimNotify_ToyOgre(FName EventName);


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
};
