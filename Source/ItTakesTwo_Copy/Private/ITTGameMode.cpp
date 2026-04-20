#include "ITTGameMode.h"
#include "Shared/ITTGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void AITTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	UClass* PawnClassToSpawn = nullptr;

	// IsLocalController()가 true면 해당 서버(리슨 서버)의 주인, 즉 호스트입니다.
	if (NewPlayer->IsLocalController())
	{
		// 호스트가 골랐던 캐릭터 클래스를 할당
		PawnClassToSpawn = (GI->HostSelectedRole == EPlayerRole::May) ? MayCharacterClass : CodyCharacterClass;
	}
	else
	{
		// 클라이언트가 골랐던 캐릭터 클래스를 할당
		PawnClassToSpawn = (GI->ClientSelectedRole == EPlayerRole::May) ? MayCharacterClass : CodyCharacterClass;
	}

	// 만약 정상적으로 선택된 캐릭터가 셋팅되어 있다면 (로비를 거치지 않고 강제 진입시엔 스폰 안함)
	if (PawnClassToSpawn)
	{
		// 맵에 배치된 PlayerStart 중 적절한 위치 하나를 가져옵니다. (기본 동작 재현)
		FTransform SpawnTransform = FTransform::Identity;
		AActor* BestStart = ChoosePlayerStart(NewPlayer);
		if (BestStart)
		{
			SpawnTransform = BestStart->GetActorTransform();
		}

		// 해당 위치에 선택된 클래스의 캐릭터를 스폰합니다.
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClassToSpawn, SpawnTransform);
		if (NewPawn)
		{
			// 게임모드가 기본으로 씌워둔 깡통(DefaultPawn)가 있다면 버리고, 새 캐릭터에 빙의(Possess).
			APawn* OldPawn = NewPlayer->GetPawn();
			NewPlayer->Possess(NewPawn);
			if (OldPawn)
			{
				OldPawn->Destroy();
			}
		}
	}
}
