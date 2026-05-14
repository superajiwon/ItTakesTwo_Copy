#pragma once
#include "CoreMinimal.h"
#include "SoundInfo.generated.h"


UENUM(BlueprintType)
enum class EITTSoundCategory : uint8
{
	BGM,
	SFX,
	UI
};


USTRUCT(BlueprintType)
struct FITTSoundInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SoundId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EITTSoundCategory Category{EITTSoundCategory::SFX};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound{nullptr};
};

