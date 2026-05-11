// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/ITTGameInstance.h"
#include "MoviePlayer.h"
#include "EngineUtils.h"
#include "Shared/Actor/PreRenderVFX.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "UObject/UObjectGlobals.h"

void UITTGameInstance::Init()
{
	Super::Init();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UITTGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UITTGameInstance::EndLoadingScreen);

}

void UITTGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Shutdown();
}

void UITTGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (IsRunningDedicatedServer())
		return;

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

		PreRenderVFX->OnPreRenderFinished.AddDynamic(
			this,
			&UITTGameInstance::FinishLoadingScreenAfterPreRenderVFX
		);

		PreRenderVFX->StartPreRender();
		return;
	}

	FinishLoadingScreenAfterPreRenderVFX();
}

void UITTGameInstance::FinishLoadingScreenAfterPreRenderVFX()
{
	if (IsRunningDedicatedServer())
		return;

	if (GetMoviePlayer() && GetMoviePlayer()->IsMovieCurrentlyPlaying())
	{
		GetMoviePlayer()->StopMovie();
	}
}
