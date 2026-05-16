// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/ITTPlayerController.h"
#include "Shared/ITTGameInstance.h"
#include "UI/UIManager/UIMangerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"

AITTPlayerController::AITTPlayerController()
{
	
}
void AITTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
		return;

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	if (UITTGameInstance* ITTGI = GetGameInstance<UITTGameInstance>())
	{
		ITTGI->OnDungeonLoadingFinished.AddUObject(
			this,
			&AITTPlayerController::DungeonLoadingFinished
		);

		GetWorldTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateWeakLambda(this, [this, ITTGI]()
			{
				if (!GetWorld())
					return;

				const FString MapName = GetWorld()->GetMapName();
				const bool bIsDungeonMap =
					MapName.EndsWith(TEXT("Lv_Dungeon")) ||
					MapName.EndsWith(TEXT("Lv_CowDungeon"));

				if (bIsDungeonMap && !ITTGI->IsGameplayPausedForLoading())
				{
					DungeonLoadingFinished(GetWorld());
				}
			})
		);
	}
}

void AITTPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UITTGameInstance* ITTGI = GetGameInstance<UITTGameInstance>())
	{
		// SeamlessTravel/레벨 종료 시 남은 delegate 바인딩을 정리
		ITTGI->OnDungeonLoadingFinished.RemoveAll(this);
	}
	if (UUIMangerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIMangerSubsystem>())
	{
		UIManager->EndHUD();
		bHUDCreated = false;
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
	
	// BGM 실행
	if (USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>())
	{
		const FString MapName = LoadedWorld->GetMapName();
		if (MapName.EndsWith(TEXT("Lv_Dungeon")))
		{
			SoundManager->PlayBGM(TEXT("BGM_Cave"));
		}
		else if (MapName.EndsWith(TEXT("Lv_CowDungeon")))
		{
			SoundManager->PlayBGM(TEXT("BGM_Cow"));
		}
	}
	
	// 던전 로딩이 끝났다는 상태를 저장하고 HUD 생성을 시도합니다.
	bDungeonLoadingFinished = true;
	TryCreateHUD();
	
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
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
