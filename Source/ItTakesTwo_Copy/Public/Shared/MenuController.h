// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Shared/ITTGameInstance.h"
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
	
	// 캐릭터를 선택할 때 호출 (클라이언트 -> 서버)
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Menu|Lobby")
	void Server_SelectCharacter(EPlayerRole SelectedRole);

	// 게임(던전 레벨) 시작 시 호출 (호스트 -> 서버)
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Menu|Lobby")
	void Server_StartGame();

	// 선택 현황을 모든 클라이언트에게 뿌려줌 (서버 -> 모든 클라이언트)
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Menu|Lobby")
	void Client_UpdateSelectionUI(EPlayerRole HostRole, EPlayerRole ClientRole);

	UFUNCTION(BlueprintCallable, Category="Network")
	void ExecuteServerTravel(FString MapName);
	
protected:
	// BP에서 이벤트 노드로 UI를 갱신할 수 있도록 마련한 함수 (초상화 잠금 처리 등)
	UFUNCTION(BlueprintImplementableEvent, Category = "Menu|Lobby")
	void OnSelectionUpdated(EPlayerRole HostRole, EPlayerRole ClientRole);
};
