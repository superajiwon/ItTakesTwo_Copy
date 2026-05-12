// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/ITTGameInstance.h"
#include "MoviePlayer.h"
#include "EngineUtils.h"
#include "Shared/Actor/PreRenderVFX.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "UObject/UObjectGlobals.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/LocalPlayer.h"


void UITTGameInstance::Init()
{
	Super::Init();
	// MoviePlayer 방식은 PIE/Standalone 멀티 창에서 불안정
	// UMG 로딩은 맵 로드 완료 후 실행되므로 PIE/Standalone/패키징에서 같은 흐름으로 동작하기 때문에 변경함
	// FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UITTGameInstance::BeginLoadingScreen);

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UITTGameInstance::EndLoadingScreen);

}

void UITTGameInstance::Shutdown()
{
	// UMG 로딩 방식을 사용하므로 MoviePlayer용 PreLoadMap 등록 해제는 사용X.
	// FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Shutdown();
}

void UITTGameInstance::BeginLoadingScreen(const FString& MapName)
{
	// UMG 로딩 방식을 사용하므로 MoviePlayer 로딩 화면은 실행X
	// 나중에 패키징 전용 로딩 화면이 필요하면 그때 다시 활성화
	return;
	
	if (IsRunningDedicatedServer())
		return;

	if (UWorld* World = GetWorld())
	{
		if (World->WorldType == EWorldType::PIE)
		{
			return;
		}
	}

	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = 1.0f;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.bWaitForManualStop = true;
	LoadingScreen.bMoviesAreSkippable = false;
	
	LoadingScreen.WidgetLoadingScreen =
	SNew(SBorder)
	.BorderBackgroundColor(FLinearColor::Black)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Loading...")))
		.ColorAndOpacity(FLinearColor::White)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 32))
	];
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	GetMoviePlayer()->PlayMovie();
	
	
	
}

void UITTGameInstance::EndLoadingScreen(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		FinishLoadingScreenAfterPreRenderVFX();
		return;
	}
	
	if (!ShouldShowLoadingForWorld(LoadedWorld))
	{
		return;
	}
	
	ShowLoadingWidget(LoadedWorld);
	SetLocalInputEnabled(LoadedWorld, false);

	LoadedWorld->GetTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(
			this,
			&UITTGameInstance::StartPreRenderVFX,
			LoadedWorld
		)
	);
}



void UITTGameInstance::StartPreRenderVFX(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		FinishLoadingScreenAfterPreRenderVFX();
		return;
	}

	for (TActorIterator<APreRenderVFX> It(LoadedWorld); It; ++It)
	{
		APreRenderVFX* PreRenderVFX = *It;
		if (!PreRenderVFX)
			continue;

		PreRenderVFX->OnPreRenderFinished.AddUniqueDynamic(this,&UITTGameInstance::FinishLoadingScreenAfterPreRenderVFX);

		PreRenderVFX->StartPreRender();
		return;
	}

	FinishLoadingScreenAfterPreRenderVFX();
}

void UITTGameInstance::FinishLoadingScreenAfterPreRenderVFX()
{
	if (IsRunningDedicatedServer())
		return;

	// UMG 로딩은 PIE에서도 동일하게 동작해야함 따라서 예외처리 X
	// if (UWorld* World = GetWorld())
	// {
	// 	if (World->WorldType == EWorldType::PIE)
	// 	{
	// 		return;
	// 	}
	// }
	
	// UMG 로딩 방식 종료 처리
	UWorld* World = GetWorld();
	if (!World)
	{
		HideLoadingWidget();
		return;
	}
	World->GetTimerManager().SetTimer(
		LoadingHideTimerHandle,
		this,
		&UITTGameInstance::FinishLoadingAfterDelay,
		LoadingHideDelay,
		false
	);
	//UMG 로딩 방식을 사용하므로 MoviePlayer 종료 처리 사용 X
	// if (GetMoviePlayer() && GetMoviePlayer()->IsMovieCurrentlyPlaying())
	// {
	// 	GetMoviePlayer()->StopMovie();
	// }
}

void UITTGameInstance::ShowLoadingWidget(UWorld* World)
{
	if (!World || !LoadingWidgetClass)
		return;

	// APlayerController* PC = World->GetFirstPlayerController();
	// if (!PC)
	// 	return;

	if (!LoadingWidget)
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
	}

	if (LoadingWidget && !LoadingWidget->IsInViewport())
	{
		LoadingWidget->AddToViewport(9999);
	}
}

void UITTGameInstance::HideLoadingWidget()
{
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
	}
}

void UITTGameInstance::SetLocalInputEnabled(UWorld* World, bool bEnabled)
{
	if (!World)
		return;

	ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
	if (!LocalPlayer)
		return;

	APlayerController* PC = LocalPlayer->GetPlayerController(World);
	if (!PC)
		return;

	PC->SetIgnoreMoveInput(!bEnabled);
	PC->SetIgnoreLookInput(!bEnabled);

	if (APawn* Pawn = PC->GetPawn())
	{
		if (bEnabled)
		{
			Pawn->EnableInput(PC);
		}
		else
		{
			Pawn->DisableInput(PC);
		}
	}
}

void UITTGameInstance::FinishLoadingAfterDelay()
{
	HideLoadingWidget();

	if (UWorld* World = GetWorld())
	{
		SetLocalInputEnabled(World, true);
	}
}

bool UITTGameInstance::ShouldShowLoadingForWorld(UWorld* LoadedWorld) const
{
	if (!LoadedWorld)
		return false;

	const FString MapName = LoadedWorld->GetMapName();

	return MapName.EndsWith(TEXT("Lv_Dungeon"))
		|| MapName.EndsWith(TEXT("Lv_CowDungeon"));
}
