

#include "Actors/Characters/Players/ITTPlayerState.h"

#include "Net/UnrealNetwork.h"


void AITTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AITTPlayerState, PlayerRole);
	DOREPLIFETIME(AITTPlayerState, PlayerSlot);
}
