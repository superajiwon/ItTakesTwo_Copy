// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Subsystems/SoundManagerSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SoundRegistry = LoadObject<USoundRegistryDataAsset>(
		nullptr,
		TEXT("/Game/Sounds/_DataAsset/DA_SoundRegistry.DA_SoundRegistry")
	);
	
	BuildSoundMap();
}

void USoundManagerSubsystem::PlayBGM(FName SoundId, bool bRestartIfSame)
{
	if (!GetWorld())
		return;

	const FITTSoundInfo* Info = FindSound(SoundId);
	if (!Info || !Info->Sound || Info->Category != EITTSoundCategory::BGM)
		return;

	if (!bRestartIfSame && CurrentBGMId == SoundId && BGMComponent && BGMComponent->IsPlaying())
		return;
	
	StopBGM();
	
	BGMComponent = UGameplayStatics::CreateSound2D(GetWorld(),
		Info->Sound,
	1.0f,
	1.0f,
	0.0f,
	nullptr, 
	true,  
	false
	);
	
	if (!BGMComponent)
		return;

	CurrentBGMId = SoundId;
	BGMComponent->bIsUISound = true;
	BGMComponent->Play();
}

void USoundManagerSubsystem::StopBGM()
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent = nullptr;
	}
	CurrentBGMId = NAME_None;
}


void USoundManagerSubsystem::PlaySFX2D(FName SoundId)
{
	if (!GetWorld())
		return;

	const FITTSoundInfo* Info = FindSound(SoundId);
	if (!Info || !Info->Sound || Info->Category != EITTSoundCategory::SFX)
		return;

	UGameplayStatics::PlaySound2D(GetWorld(), Info->Sound);
}

void USoundManagerSubsystem::PlaySFX3D(FName SoundId, FVector Location)
{
	if (!GetWorld())
		return;
	
	const FITTSoundInfo* Info = FindSound(SoundId);
	if (!Info || !Info->Sound)
		return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Info->Sound, Location);
}

void USoundManagerSubsystem::PlayUI(FName SoundId)
{
	if (!GetWorld())
		return;
	
	const FITTSoundInfo* Info = FindSound(SoundId);
	if (!Info || !Info->Sound || Info->Category != EITTSoundCategory::UI)
		return;

	UAudioComponent* UIAudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), Info->Sound);
	if (!UIAudioComponent)
		return;

	UIAudioComponent->bIsUISound = true;
	UIAudioComponent->Play();
}

void USoundManagerSubsystem::BuildSoundMap()
{
	SoundMap.Empty();

	if (!SoundRegistry)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundRegistry is null."));
		return;
	}

	for (USoundLibraryDataAsset* Library : SoundRegistry->Libraries)
	{
		if (!Library)
			continue;

		for (const FITTSoundInfo& SoundInfo : Library->Sounds)
		{
			if (SoundInfo.SoundId.IsNone())
				continue;

			if (!SoundInfo.Sound)
			{
				UE_LOG(LogTemp, Warning, TEXT("Sound '%s' has no Sound asset."), *SoundInfo.SoundId.ToString());
				continue;
			}

			if (SoundMap.Contains(SoundInfo.SoundId))
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("Duplicated SoundId '%s' in SoundLibrary '%s'. First one will be used."),
					*SoundInfo.SoundId.ToString(),
					*Library->LibraryId.ToString()
				);
				continue;
			}

			SoundMap.Add(SoundInfo.SoundId, SoundInfo);
		}
	}
}

const FITTSoundInfo* USoundManagerSubsystem::FindSound(FName SoundId) const
{
	if (SoundId.IsNone())
		return nullptr;

	return SoundMap.Find(SoundId);
}
