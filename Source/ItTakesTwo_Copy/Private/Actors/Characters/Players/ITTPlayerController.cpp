// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/ITTPlayerController.h"
#include "Shared/ITTGameInstance.h"
#include "UI/UIManager/UIMangerSubsystem.h"
#include "EnhancedInputSubsystems.h"

AITTPlayerController::AITTPlayerController()
{
	
}

void AITTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController())
		return;

	if (UITTGameInstance* ITTGI = GetGameInstance<UITTGameInstance>())
	{
		// 로딩 완료 이벤트를 받아 HUD 생성 조건 확인
		ITTGI->OnDungeonLoadingFinished.AddUObject(this, &AITTPlayerController::DungeonLoadingFinished);
	}
}

void AITTPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UITTGameInstance* ITTGI = GetGameInstance<UITTGameInstance>())
	{
		// SeamlessTravel/레벨 종료 시 남은 delegate 바인딩을 정리
		ITTGI->OnDungeonLoadingFinished.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AITTPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	// Pawn 소유가 완료된 시점에도 로딩 완료 여부를 함께 확인합니다.
	TryCreateHUD();
}

void AITTPlayerController::DungeonLoadingFinished(UWorld* LoadedWorld)
{
	if (!IsLocalController())
		return;

	if (LoadedWorld != GetWorld())
		return;

	// 던전 로딩이 끝났다는 상태를 저장하고 HUD 생성을 시도합니다.
	bDungeonLoadingFinished = true;
	TryCreateHUD();
}

void AITTPlayerController::TryCreateHUD()
{
	if (!IsLocalController())
		return;

	if (bHUDCreated)
		return;

	if (!bDungeonLoadingFinished)
		return;

	if (!GetPawn())
		return;

	if (UUIMangerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIMangerSubsystem>())
	{
		UIManager->BeginHUD(this);
		bHUDCreated = true;
	}
}
