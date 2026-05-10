// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/CullingBoundBox.h"

#include "EngineUtils.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"

ACullingBoundBox::ACullingBoundBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Tags.AddUnique(TEXT("NoSectionCull"));
	SectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SectionBox"));
	SectionBox->ShapeColor = FColor::Green;
	
	RootComponent = SectionBox;

	SectionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SectionBox->SetGenerateOverlapEvents(false);
	
}

void ACullingBoundBox::BeginPlay()
{
	Super::BeginPlay();
	
	FindOverlapActor();
	
	
}

void ACullingBoundBox::FindOverlapActor()
{
	SectionActors.Empty();

	if (!SectionBox || !GetWorld())
		return;

	const FBox SectionBounds = SectionBox->Bounds.GetBox();

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor || Actor == this)
			continue;
		if (Actor->IsA<APlayerBase>())
			continue;

		if (HasIgnoreTags(Actor))
			continue;
		
		if (!Actor->ActorHasTag(TEXT("SectionCull")))
			continue;
		
		bool bIsInsideSection = false;

		if (Actor->FindComponentByClass<ULightComponent>())
		{
			bIsInsideSection = SectionBounds.IsInside(Actor->GetActorLocation());
		}
		else
		{
			FVector Origin;
			FVector Extent;
			Actor->GetActorBounds(false, Origin, Extent);

			const FBox ActorBounds(Origin - Extent, Origin + Extent);
			bIsInsideSection = SectionBounds.Intersect(ActorBounds);
		}

		if (bIsInsideSection)
		{
			SectionActors.Add(Actor);
		}
	}
}

void ACullingBoundBox::SetActiveOn()
{
	bSectionActive = true;

	for (AActor* Actor : SectionActors)
	{
		SetActiveActor(Actor, true);
	}
}

void ACullingBoundBox::SetActiveOff()
{
	bSectionActive = false;

	for (AActor* Actor : SectionActors)
	{
		SetActiveActor(Actor, false);
	}
}

void ACullingBoundBox::SetActiveActor(AActor* Actor, bool bActive)
{
	if (!Actor)
		return;

	Actor->SetActorHiddenInGame(!bActive);

	TArray<ULightComponent*> LightComponents;
	Actor->GetComponents<ULightComponent>(LightComponents);

	for (ULightComponent* LightComp : LightComponents)
	{
		if (!LightComp)
			continue;

		LightComp->SetVisibility(bActive, true);
	}

	if (Actor->ActorHasTag(TEXT("SectionCullCollision")))
	{
		Actor->SetActorEnableCollision(bActive);
	}

	if (Actor->ActorHasTag(TEXT("SectionCullTick")))
	{
		Actor->SetActorTickEnabled(bActive);

		TArray<UActorComponent*> Components = Actor->GetComponents().Array();
		for (UActorComponent* Component : Components)
		{
			if (Component)
			{
				Component->SetComponentTickEnabled(bActive);
			}
		}
	}
}

bool ACullingBoundBox::CheckOverlapLocation(FVector Location)
{
	if (!SectionBox)
		return false;

	return SectionBox->Bounds.GetBox().IsInside(Location);
}

bool ACullingBoundBox::HasIgnoreTags(AActor* Actor)
{
	// 태그를 갖고 있으면 넘어가라
	for (const FName& IgnoreTag : IgnoreTags)
	{
		if (Actor->ActorHasTag(IgnoreTag))
			return true;
	}
	return false;
}

