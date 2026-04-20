// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/ViewModelBase.h"

void UViewModelBase::Initialize(TObjectPtr<APlayerController> PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is NULL"));
		return;
	}
	
	MyPlayerController = PlayerController;
	
}

void UViewModelBase::Deinitialize()
{
	MyPlayerController = nullptr;
	
}
