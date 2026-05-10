// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/CullingSection_EntranceBox.h"

#include "EngineUtils.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Shared/Actor/CullingBoundBoxManager.h"


ACullingSection_EntranceBox::ACullingSection_EntranceBox()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique(TEXT("NoSectionCull"));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	TriggerBox->SetCollisionProfileName(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetGenerateOverlapEvents(true);
	
	
}

void ACullingSection_EntranceBox::BeginPlay()
{
	Super::BeginPlay();
	
	if (!CullingManager)
	{
		for (TActorIterator<ACullingBoundBoxManager> It(GetWorld()); It; ++It)
		{
			CullingManager = *It;
			break;
		}
	}

	if (TriggerBox)
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACullingSection_EntranceBox::OnTriggerBeginOverlap);
	
}


void ACullingSection_EntranceBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!OtherActor)
		return;

	if (!OtherActor->ActorHasTag(PlayerTag))
		return;

	if (!CullingManager)
		return;

	if (SectionIndex < 0)
		return;
	if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
	{
		CullingManager->PlayerSectionChange(Player, SectionIndex);
	}
}

