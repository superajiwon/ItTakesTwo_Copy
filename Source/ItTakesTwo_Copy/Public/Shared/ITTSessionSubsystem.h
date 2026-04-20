#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ITTSessionSubsystem.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API UITTSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	// 여기서 세션 생성, 찾기, 참여를 하자
public:
	void CreateSession(int32 NumPublicConnection);
	void JoinSession();
	void FindSession();
	
private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	
	UPROPERTY()
	FName GameSessionName{NAME_GameSession};
	
	UPROPERTY()
	FName ListenLevelName{FName("Lv_ServerTestInGame")};
	
};
