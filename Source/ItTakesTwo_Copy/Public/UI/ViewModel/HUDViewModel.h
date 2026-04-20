// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItTakesTwo_Copy/Public/UI/ViewModel/ViewModelBase.h"
#include "HUDViewModel.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UHUDViewModel : public UViewModelBase
{
	GENERATED_BODY()
public:
	virtual void Initialize(TObjectPtr<APlayerController> PlayerController) override;
	virtual void Deinitialize() override;
	
	
	
	
};
