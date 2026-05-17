
#include "Actors/Characters/Players/Rose/RoseCharacter.h"

#include "Actors/Characters/Players/Rose/RoseAnimInstance.h"


ARoseCharacter::ARoseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARoseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARoseCharacter::Multicast_Selected_Implementation()
{
	URoseAnimInstance* Anim = Cast<URoseAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->Select();
	}
}

