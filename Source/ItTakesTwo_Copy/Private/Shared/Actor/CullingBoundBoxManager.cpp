// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/CullingBoundBoxManager.h"
#include "EngineUtils.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Shared/Actor/CullingBoundBox.h"
#include "TimerManager.h"

ACullingBoundBoxManager::ACullingBoundBoxManager()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique(TEXT("NoSectionCull"));
	
}

void ACullingBoundBoxManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle InitTimerHandle;
	GetWorldTimerManager().SetTimer(
		InitTimerHandle,
		this,
		&ACullingBoundBoxManager::InitializeCulling,
		0.2f,
		false
	);

}

void ACullingBoundBoxManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void ACullingBoundBoxManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACullingBoundBoxManager::PlayerSectionChange(APlayerBase* PlayerActor, int32 NewSectionIndex)
{
	if (!PlayerActor || NewSectionIndex < 0)
		return;

	const int32* PrevSectionIndex = PlayerSectionMap.Find(PlayerActor);
	if (PrevSectionIndex && *PrevSectionIndex == NewSectionIndex)
		return;

	PlayerSectionMap.FindOrAdd(PlayerActor) = NewSectionIndex;

	VisitedIndexes.Add(NewSectionIndex);

	ReCheckActiveSections();

}

void ACullingBoundBoxManager::InitializeCulling()
{
	GetAllSections();
	ActivateStartSection();
}

void ACullingBoundBoxManager::GetAllSections()
{
	if (Sections.Num() <= 0 && GetWorld())
	{
		for (TActorIterator<ACullingBoundBox> It(GetWorld()); It; ++It)
		{
			Sections.Add(*It);
		}
	}

	for (ACullingBoundBox* Section : Sections)
	{
		if (Section)
		{
			Section->FindOverlapActor();
		}
	}
}


void ACullingBoundBoxManager::ReCheckActiveSections()
{
	TSet<int32> NewActiveSectionIndexes;

	for (const TPair<TObjectPtr<AActor>, int32>& Pair : PlayerSectionMap)
	{
		const int32 SectionIndex = Pair.Value;

		ACullingBoundBox* Section = FindSectionByIndex(SectionIndex);
		if (!Section)
			continue;

		AddRenderSection(Section, NewActiveSectionIndexes);
	}

	for (const int32 VisitedIndex : VisitedIndexes)
	{
		NewActiveSectionIndexes.Add(VisitedIndex);
	}
	
	bool bSame = NewActiveSectionIndexes.Num() == CurrentActiveSectionIndexes.Num();
	if (bSame)
	{
		for (const int32 SectionIndex : NewActiveSectionIndexes)
		{
			if (!CurrentActiveSectionIndexes.Contains(SectionIndex))
			{
				bSame = false;
				break;
			}
		}
	}
	if (bSame)
		return;

	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;
		const int32 SectionIndex = Section->GetSectionIndex();
		if (!NewActiveSectionIndexes.Contains(SectionIndex))
			Section->SetActiveOff();
	}

	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;
		const int32 SectionIndex = Section->GetSectionIndex();

		if (NewActiveSectionIndexes.Contains(SectionIndex))
		{
			Section->SetActiveOn();
		}
	}
	CurrentActiveSectionIndexes = NewActiveSectionIndexes;
}

void ACullingBoundBoxManager::ActivateStartSection()
{
	ACullingBoundBox* StartSection = FindSectionByIndex(StartSectionIndex);
	if (!StartSection)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSection not found. Index=%d"), StartSectionIndex);
		return;
	}

	// 시작 섹션만 방문 처리
	VisitedIndexes.Add(StartSectionIndex);

	TSet<int32> NewActiveSectionIndexes;
	AddRenderSection(StartSection, NewActiveSectionIndexes);

	for (const int32 VisitedIndex : VisitedIndexes)
	{
		NewActiveSectionIndexes.Add(VisitedIndex);
	}

	// Off 먼저
	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;

		const int32 SectionIndex = Section->GetSectionIndex();

		if (!NewActiveSectionIndexes.Contains(SectionIndex))
		{
			Section->SetActiveOff();
		}
	}

	// On 나중
	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;

		const int32 SectionIndex = Section->GetSectionIndex();

		if (NewActiveSectionIndexes.Contains(SectionIndex))
		{
			Section->SetActiveOn();
		}
	}

	CurrentActiveSectionIndexes = NewActiveSectionIndexes;
}

void ACullingBoundBoxManager::AddRenderSection(ACullingBoundBox* BoundBox, TSet<int32>& NewSections)
{
	const int32 SectionIndex = BoundBox->GetSectionIndex();
	if (SectionIndex < 0)
		return;

	NewSections.Add(SectionIndex);

	for (const int32 NeighborIndex : BoundBox->GetNeighborIndex())
	{
		if (FindSectionByIndex(NeighborIndex))
		{
			NewSections.Add(NeighborIndex);
		}
	}
}

ACullingBoundBox* ACullingBoundBoxManager::FindSectionByIndex(int32 SectionIndex) const
{
	for (ACullingBoundBox* Section : Sections)
	{
		if (Section && Section->GetSectionIndex() == SectionIndex)
		{
			return Section;
		}
	}
	return nullptr;
}

