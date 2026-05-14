// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Shared/Struct/SoundInfo.h"
#include "SoundLibraryDataAsset.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API USoundLibraryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LibraryId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FITTSoundInfo> Sounds{};
};
