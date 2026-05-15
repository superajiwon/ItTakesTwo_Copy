
#include "ITTGameMode.h"

#include "EngineUtils.h"
#include "Actors/Characters/Players/ITTPlayerState.h"
#include "Shared/ITTGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"


void AITTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	
}
void AITTGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (!NewPlayer) return;

	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	TSubclassOf<APawn> PawnClassToSpawn = nullptr;

	// 서버에서 이 플레이어의 세션 슬롯과 캐릭터 역할을 확정합니다.
	const bool bIsHostPlayer = NewPlayer->IsLocalController();

	const EPlayerSlot SelectedSlot = bIsHostPlayer
		? EPlayerSlot::Host
		: EPlayerSlot::Client;

	const EPlayerRole SelectedRole = bIsHostPlayer
		? GI->HostSelectedRole
		: GI->ClientSelectedRole;

	if (AITTPlayerState* ITTPS = NewPlayer->GetPlayerState<AITTPlayerState>())
	{
		ITTPS->PlayerSlot = SelectedSlot;
		ITTPS->PlayerRole = SelectedRole;
	}

	PawnClassToSpawn = SelectedRole == EPlayerRole::May
		? MayCharacterClass
		: CodyCharacterClass;

	UE_LOG(LogTemp, Warning, TEXT("[ITTGameMode] Start Player=%s IsLocal=%d HostRole=%d ClientRole=%d PawnClass=%s"),
		*GetNameSafe(NewPlayer),
		NewPlayer->IsLocalController(),
		(int32)GI->HostSelectedRole,
		(int32)GI->ClientSelectedRole,
		*GetNameSafe(PawnClassToSpawn));

	if (!PawnClassToSpawn) return;

	FTransform SpawnTransform = FTransform::Identity;
	if (AActor* BestStart = ChoosePlayerStart(NewPlayer))
	{
		SpawnTransform = BestStart->GetActorTransform();
	}

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClassToSpawn, SpawnTransform);
	if (!NewPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[ITTGameMode] SpawnActor failed"));
		return;
	}

	NewPlayer->Possess(NewPawn);

	UE_LOG(LogTemp, Warning, TEXT("[ITTGameMode] Pawn after start: %s"), *GetNameSafe(NewPlayer->GetPawn()));
}

AActor* AITTGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AITTPlayerState* PS = Player->GetPlayerState<AITTPlayerState>();
	if (!PS) return Super::ChoosePlayerStart_Implementation(Player);
	
	FName TargetTag = (PS->PlayerRole == EPlayerRole::May) ? FName("May") : FName("Cody");
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start && Start->PlayerStartTag == TargetTag)
		{
			return Start;
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AITTGameMode::ChangeLevel(const FString& LevelPath)
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	bUseSeamlessTravel = true;
	// World->ServerTravel(TEXT("/Game/Maps/Lv_CowDungeon"), true);
	const FString TravelURL = LevelPath + TEXT("?listen");
	World->ServerTravel(TravelURL, true);
}

void AITTGameMode::UpdateRespawnPoint(ARespawn_TargetPoint* NewRespawnPointCody, ARespawn_TargetPoint* NewRespawnPointMay)
{
	CurRespawnPointCody = NewRespawnPointCody;
	CurRespawnPointMay = NewRespawnPointMay;
}

FTransform AITTGameMode::GetRespawnTransform(APawn* Player) const
{
	if (!Player) return FTransform::Identity;

	if (Player->IsA(MayCharacterClass))
	{
		return CurRespawnPointMay ? CurRespawnPointMay->GetActorTransform() : FTransform::Identity;
	}
	else if (Player->IsA(CodyCharacterClass))
	{
		return CurRespawnPointCody ? CurRespawnPointCody->GetActorTransform() : FTransform::Identity;
	}
	
	return FTransform::Identity;
}

