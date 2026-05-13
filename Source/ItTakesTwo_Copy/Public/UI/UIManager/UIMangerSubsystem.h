// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIMangerSubsystem.generated.h"


class ULobbyViewModel;
class UHUDViewModel;

UCLASS()
class ITTAKESTWO_COPY_API UUIMangerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	

	// WBP와 ViewModel은 OpenLevel 할 때마다 재생성하는데 같은 레벨에선 재사용하게 만들 예정
public: // UI 시작, ViewModel & WBP 생성
	void BeginLobbyUI();
	void BeginHUD(APlayerController* OwningPC);
	
	
public: // 레벨 전환 시 바인딩한 ViewModel, WBP 삭제로직
	void EndLobbyUI();
	void EndHUD();
	
	
	
private:
	UPROPERTY()
	TObjectPtr<UHUDViewModel> HUDViewModel;
	
	UPROPERTY()
	TObjectPtr<ULobbyViewModel> LobbyViewModel;
	
	
};
