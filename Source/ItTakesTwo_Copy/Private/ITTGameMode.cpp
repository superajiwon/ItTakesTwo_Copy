
#include "ITTGameMode.h"
#include "Shared/ITTGameInstance.h"
#include "GameFramework/PlayerController.h"


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

	if (NewPlayer->IsLocalController())
	{
		PawnClassToSpawn = (GI->HostSelectedRole == EPlayerRole::May)
			? MayCharacterClass
			: CodyCharacterClass;
	}
	else
	{
		PawnClassToSpawn = (GI->ClientSelectedRole == EPlayerRole::May)
			? MayCharacterClass
			: CodyCharacterClass;
	}

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

