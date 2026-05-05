// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ToyOgre_StateBase.generated.h"

class AToyOgre_Monster;

UCLASS(Abstract) // 추상클래스
class ITTAKESTWO_COPY_API UToyOgre_StateBase : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(AToyOgre_Monster* InOwner)
	{
		OwnerOgre = InOwner;
	}

	virtual void Enter() {}
	virtual void Tick(float DeltaTime) {}
	virtual void Exit() {}

	virtual void HandleAnimNotify(FName NotifyName) {}
	
	
protected:
	UPROPERTY()
	TObjectPtr<AToyOgre_Monster> OwnerOgre;
	
	
};
