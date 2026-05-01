
#include "Actors/Characters/Players/Cody/CodyAnimInstance.h"
#include "Actors/Characters/Players/Cody/CodyCharacter.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Components/HitBoxComponent.h"


void UCodyAnimInstance::AnimNotify_DashOn()
{
	Super::AnimNotify_DashOn();
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	// 애니메이션 타이밍에 맞춰서 정확하게 코디를 순간이동시킵니다.
	Owner->CodyTeleport(Owner->TeleportLength);

	if (Owner->GetLocalRole() < ROLE_Authority)
	{
		Owner->Server_CodyTeleport();
	}
}

void UCodyAnimInstance::AnimNotify_SpecialOn()
{
	Super::AnimNotify_SpecialOn();
	
}

void UCodyAnimInstance::AnimNotify_UltimateOn()
{
	Super::AnimNotify_UltimateOn();
	
	auto* Owner = Cast<ACodyCharacter>(GetOwningActor());
	if (!Owner) return;
	
	int32 RandDamage = FMath::RandRange(5, 18);
	Owner->UltimateCollision->SetDamage(RandDamage);
	Owner->UltimateCollision->SetHiddenInGame(false);
	Owner->UltimateCollision->CollisionOn();
}

