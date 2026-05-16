
#include "UI/Actor/FloatingUIActor.h"
#include "Components/WidgetComponent.h"
#include "UI/View/FloatingUI.h"


AFloatingUIActor::AFloatingUIActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
	
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void AFloatingUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (WidgetComp)
	{
		CachedFloatingUI = Cast<UFloatingUI>(WidgetComp->GetUserWidgetObject());
	}
}

void AFloatingUIActor::ShowFloatingUI(FText Text, FLinearColor Color)
{
	if (UFloatingUI* FloatingUI = Cast<UFloatingUI>(WidgetComp->GetUserWidgetObject()))
	{
		FloatingUI->PlayFloating(Text, Color);
	}

	SetLifeSpan(1.0f);
}

void AFloatingUIActor::ActivateFloatingUI(FText Text, FLinearColor Color)
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(true);
	}
	
	if (CachedFloatingUI)
	{
		CachedFloatingUI->PlayFloating(Text, Color);
	}
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AFloatingUIActor::DeactivateFloatingUI, 1.0f, false);
}

void AFloatingUIActor::DeactivateFloatingUI()
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	if (WidgetComp)
	{
		WidgetComp->SetVisibility(false);
	}
}
