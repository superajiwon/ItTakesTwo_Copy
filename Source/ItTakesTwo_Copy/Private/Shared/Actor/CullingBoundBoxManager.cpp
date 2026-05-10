// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/CullingBoundBoxManager.h"
#include "EngineUtils.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Shared/Actor/CullingBoundBox.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

ACullingBoundBoxManager::ACullingBoundBoxManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

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
	if (!HasAuthority())
		return;

	if (!PlayerActor || NewSectionIndex < 0)
		return;

	const int32* PrevSectionIndex = PlayerSectionMap.Find(PlayerActor);
	if (PrevSectionIndex && *PrevSectionIndex == NewSectionIndex)
		return;

	PlayerSectionMap.FindOrAdd(PlayerActor) = NewSectionIndex;
	VisitedIndexes.Add(NewSectionIndex);

	CalculateActiveSectionsFromPlayerSectionMap();
}

void ACullingBoundBoxManager::CalculateActiveSectionsFromPlayerSectionMap()
{
	if (!HasAuthority())
		return;

	TSet<int32> NewActiveSectionIndexes;

	for (const TPair<TObjectPtr<AActor>, int32>& Pair : PlayerSectionMap)
	{
		if (!Pair.Key)
			continue;

		const int32 CurrentSectionIndex = Pair.Value;

		ACullingBoundBox* CurrentSection = FindSectionByIndex(CurrentSectionIndex);
		if (!CurrentSection)
			continue;

		AddRenderSection(CurrentSection, NewActiveSectionIndexes);
	}

	RepActiveSectionIndexes = NewActiveSectionIndexes.Array();
	ApplyActiveSections(NewActiveSectionIndexes);
}

void ACullingBoundBoxManager::InitializeCulling()
{
	GetAllSections();

	if (HasAuthority())
	{
		RefreshPlayerSectionsByLocation();
		CalculateActiveSectionsFromPlayerSectionMap();
	}
	else
	{
		OnRep_ActiveSectionIndexes();
	}
}

void ACullingBoundBoxManager::GetAllSections()
{
	Sections.Empty();

	if (GetWorld())
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
		AActor* PlayerActor = Pair.Key;
		if (!PlayerActor)
			continue;
		const int32 CurrentSectionIndex = Pair.Value;
		VisitedIndexes.Add(CurrentSectionIndex);
		ACullingBoundBox* CurrentSection = FindSectionByIndex(CurrentSectionIndex);
		if (!CurrentSection)
			continue;
		AddRenderSection(CurrentSection, NewActiveSectionIndexes);
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
		{
			Section->SetActiveOff();
		}
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

	VisitedIndexes.Add(StartSectionIndex);

	TSet<int32> NewActiveSectionIndexes;
	AddRenderSection(StartSection, NewActiveSectionIndexes);

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
	if (!BoundBox)
		return;

	const int32 SectionIndex = BoundBox->GetSectionIndex();
	if (SectionIndex < 0)
		return;

	NewSections.Add(SectionIndex);

	for (const int32 NeighborIndex : BoundBox->GetNeighborIndex())
	{
		if (!VisitedIndexes.Contains(NeighborIndex))
			continue;

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

void ACullingBoundBoxManager::OnRep_ActiveSectionIndexes()
{
	TSet<int32> ActiveSet;

	for (const int32 SectionIndex : RepActiveSectionIndexes)
	{
		ActiveSet.Add(SectionIndex);
	}

	ApplyActiveSections(ActiveSet);
}

void ACullingBoundBoxManager::ApplyActiveSections(const TSet<int32>& ActiveSectionIndexes)
{
	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;

		if (!ActiveSectionIndexes.Contains(Section->GetSectionIndex()))
		{
			Section->SetActiveOff();
		}
	}

	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;

		if (ActiveSectionIndexes.Contains(Section->GetSectionIndex()))
		{
			Section->SetActiveOn();
		}
	}

	CurrentActiveSectionIndexes = ActiveSectionIndexes;
}

void ACullingBoundBoxManager::CalculateActiveSectionsFromPlayers()
{
	if (!HasAuthority())
		return;

	RefreshPlayerSectionsByLocation();

	TSet<int32> NewActiveSectionIndexes;

	for (const TPair<TObjectPtr<AActor>, int32>& Pair : PlayerSectionMap)
	{
		const int32 CurrentSectionIndex = Pair.Value;

		ACullingBoundBox* CurrentSection = FindSectionByIndex(CurrentSectionIndex);
		if (!CurrentSection)
			continue;

		AddRenderSection(CurrentSection, NewActiveSectionIndexes);
	}

	RepActiveSectionIndexes = NewActiveSectionIndexes.Array();

	ApplyActiveSections(NewActiveSectionIndexes);
}

void ACullingBoundBoxManager::RefreshPlayerSectionsByLocation()
{
	PlayerSectionMap.Empty();

	for (TActorIterator<APlayerBase> It(GetWorld()); It; ++It)
	{
		APlayerBase* Player = *It;
		if (!Player)
			continue;

		const int32 SectionIndex = FindSectionIndexByLocation(Player->GetActorLocation());
		if (SectionIndex < 0)
			continue;

		PlayerSectionMap.Add(Player, SectionIndex);
		VisitedIndexes.Add(SectionIndex);
	}
}

int32 ACullingBoundBoxManager::FindSectionIndexByLocation(const FVector& Location) const
{
	for (ACullingBoundBox* Section : Sections)
	{
		if (!Section)
			continue;

		if (Section->CheckOverlapLocation(Location))
		{
			return Section->GetSectionIndex();
		}
	}

	return INDEX_NONE;
}

void ACullingBoundBoxManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACullingBoundBoxManager, RepActiveSectionIndexes);
}