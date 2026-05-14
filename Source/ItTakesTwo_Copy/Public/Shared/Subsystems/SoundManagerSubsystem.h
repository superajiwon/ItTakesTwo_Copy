// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/DataAsset/SoundRegistryDataAsset.h"
#include "Shared/Struct/SoundInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void PlayBGM(FName SoundId, bool bRestartIfSame = false);

	UFUNCTION(BlueprintCallable)
	void PlaySFX2D(FName SoundId);

	UFUNCTION(BlueprintCallable)
	void PlaySFX3D(FName SoundId, FVector Location);

	UFUNCTION(BlueprintCallable)
	void PlayUI(FName SoundId);

	UFUNCTION(BlueprintCallable)
	void StopBGM();

	
private:
	void BuildSoundMap();
	const FITTSoundInfo* FindSound(FName SoundId) const;

private:
	UPROPERTY()
	TObjectPtr<USoundRegistryDataAsset> SoundRegistry;

	UPROPERTY()
	TMap<FName, FITTSoundInfo> SoundMap;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;

private:
	FName CurrentBGMId;
};
