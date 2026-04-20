#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shared/ITTTypes.h"
#include "ITTGameInstance.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UITTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 방장(Host)이 선택한 캐릭터 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
	EPlayerRole HostSelectedRole = EPlayerRole::None;

	// 클라이언트(Client)가 선택한 캐릭터 역할
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
	EPlayerRole ClientSelectedRole = EPlayerRole::None;
};
