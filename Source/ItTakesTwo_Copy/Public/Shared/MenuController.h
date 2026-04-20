// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API AMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
public:
	void HostStart();
	void ClientJoin();
	void FindSession();
	
};
