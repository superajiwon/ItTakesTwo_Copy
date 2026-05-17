
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ITTPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ITTAKESTWO_COPY_API AITTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AITTPlayerController();
	
	UFUNCTION(BlueprintCallable, Category="Network")
	void ExecuteServerTravel(FString MapName);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void AcknowledgePossession(APawn* P) override;

private:
	// 로딩 완료와 Pawn 준비가 모두 끝난 뒤 한 번만 HUD를 생성합니다.
	void DungeonLoadingFinished(UWorld* LoadedWorld);
	void TryCreateHUD();

	
private:
	bool bDungeonLoadingFinished = false;
	bool bHUDCreated = false;
};
