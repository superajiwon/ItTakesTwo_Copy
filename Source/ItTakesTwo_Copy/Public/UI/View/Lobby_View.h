// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lobby_View.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API ULobby_View : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	
private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* Btn_Create{};
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* Btn_Join{};
	
	UFUNCTION()
	void CreateServerHandler();
	UFUNCTION()
	void JoinServerHandler();
};
