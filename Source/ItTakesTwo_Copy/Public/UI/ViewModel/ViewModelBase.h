// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ViewModelBase.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API UViewModelBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(TObjectPtr<APlayerController> PlayerController);
	virtual void Deinitialize();
	
	
protected:
	UPROPERTY()
	TWeakObjectPtr<APlayerController> MyPlayerController;
	
	
};
