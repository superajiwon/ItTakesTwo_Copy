// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToyOgre_StateMachineComponent.generated.h"


class AToyOgre_Monster;
class UToyOgre_StateBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESTWO_COPY_API UToyOgre_StateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UToyOgre_StateMachineComponent();

public:
	void Init(AToyOgre_Monster* InOwner);
	void ChangeState(TSubclassOf<UToyOgre_StateBase> NewStateClass);
	void TickState(float DeltaTime);

	void HandleAnimNotify(FName NotifyName);
private:
	UPROPERTY()
	TObjectPtr<AToyOgre_Monster> OwnerOgre;

	UPROPERTY()
	TObjectPtr<UToyOgre_StateBase> CurrentState;
};
