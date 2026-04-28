
#include "Shared/Components/DotHitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"

UDotHitSphereComponent::UDotHitSphereComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}


// Called when the game starts
void UDotHitSphereComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UDotHitSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bCollisionOn) return;
	
	AccumulatedTime += DeltaTime;
	
	if (AccumulatedTime >= DotInterval)
	{
		// 정확한 타격 주기를 위해
		AccumulatedTime -= DotInterval;
		
		TArray<AActor*> OverlappingActors; 
		GetOverlappingActors(OverlappingActors);
		
		for (AActor* HitActor : OverlappingActors)
		{
			if (!IsValid(HitActor) || HitActor == GetOwner()) continue;
			if (TargetTag != NAME_None && !HitActor->ActorHasTag(TargetTag)) continue;
			
			if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
			{
				FHitRequest HitInfo(GetOwner(), HitActor, Damage, HitActor->GetActorLocation());
				CombatSystem->ProcessHit(HitInfo);
			}
		}
	}
}

void UDotHitSphereComponent::InitializeHitComp(FHitComp_Info HitInfo, FName TargetName)
{
	ComponentTags.Reset();
	ComponentTags.Add(HitInfo.HitTagName);
	SetSphereRadius(HitInfo.HitSphereRadius);
	SetRelativeLocation(HitInfo.HitSphereLocation);
	SetCollisionProfileName(HitInfo.CollisionProfileName);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
	TargetTag = TargetName;
}

void UDotHitSphereComponent::CollisionOn()
{
	bCollisionOn = true;
	SetGenerateOverlapEvents(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpdateOverlaps();
	
	AccumulatedTime = DotInterval;
}

void UDotHitSphereComponent::CollisionOff()
{
	bCollisionOn = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}

