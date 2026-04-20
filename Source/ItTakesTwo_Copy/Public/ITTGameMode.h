
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ITTGameMode.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AITTGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 블루프린트로 할당될 메이(May)의 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Characters")
	TSubclassOf<APawn> MayCharacterClass;

	// 블루프린트로 할당될 코디(Cody)의 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Characters")
	TSubclassOf<APawn> CodyCharacterClass;
};
