
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
