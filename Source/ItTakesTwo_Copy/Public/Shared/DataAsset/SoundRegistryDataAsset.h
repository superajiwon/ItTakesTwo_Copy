// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundLibraryDataAsset.h"
#include "Engine/DataAsset.h"
#include "SoundRegistryDataAsset.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API USoundRegistryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USoundLibraryDataAsset>> Libraries;
};