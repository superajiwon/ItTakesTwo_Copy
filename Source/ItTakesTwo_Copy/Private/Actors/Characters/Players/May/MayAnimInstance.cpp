
#include "Actors/Characters/Players/May/MayAnimInstance.h"

#include "Actors/Characters/Players/May/MayCharacter.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Components/DotHitSphereComponent.h"
#include "Shared/Components/HitSphereComponent.h"


void UMayAnimInstance::AnimNotify_DashOn()
{
	Super::AnimNotify_DashOn();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	if (Owner->IsLocallyControlled())
	{
		FVector DashDir = Owner->GetActorForwardVector();
		Owner->MayDash(DashDir, Owner->DashStrength, Owner->DashDuration);
	
		if (Owner->GetLocalRole() < ROLE_Authority)
		{
			Owner->Server_MayDash();
		}
	}
}

void UMayAnimInstance::AnimNotify_SpecialOn()
{
	Super::AnimNotify_SpecialOn();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	// Debug
	Owner->SpecialCollision->SetHiddenInGame(false);
	int32 RandDamage = FMath::RandRange(5, 18);
	Owner->SpecialCollision->SetDamage(RandDamage);
	Owner->SpecialCollision->CollisionOn();
}

void UMayAnimInstance::AnimNotify_SpecialOff()
{
	Super::AnimNotify_SpecialOff();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	// Debug
	Owner->SpecialCollision->SetHiddenInGame(true);
	Owner->SpecialCollision->CollisionOff();
}

void UMayAnimInstance::AnimNotify_UltimateOn()
{
	Super::AnimNotify_UltimateOn();
	
	auto* Owner = Cast<AMayCharacter>(GetOwningActor());
	if (!Owner) return;
	
	int32 RandDamage = FMath::RandRange(5, 18);
	Owner->UltimateCollision->SetDamage(RandDamage);
	Owner->UltimateCollision->CollisionOn();
}

