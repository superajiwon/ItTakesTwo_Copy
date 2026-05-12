
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/Respawn_TargetPoint.h"
#include "GameFramework/GameModeBase.h"
#include "ITTGameMode.generated.h"

class ARespawn_TargetPoint;

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
	
	// 현재 체크포인트 위치
	UPROPERTY(BlueprintReadWrite, Category = "RespawnPoint")
	TObjectPtr<ARespawn_TargetPoint> CurRespawnPointMay;
	UPROPERTY(BlueprintReadWrite, Category = "RespawnPoint")
	TObjectPtr<ARespawn_TargetPoint> CurRespawnPointCody;
	
	UFUNCTION(BlueprintCallable, Category = "RespawnPoint")
	void UpdateRespawnPoint(ARespawn_TargetPoint* NewRespawnPointCody, ARespawn_TargetPoint* NewRespawnPointMay);
	UFUNCTION(BlueprintCallable, Category = "RespawnPoint")
	FTransform GetRespawnTransform(APawn* Player) const;
};
