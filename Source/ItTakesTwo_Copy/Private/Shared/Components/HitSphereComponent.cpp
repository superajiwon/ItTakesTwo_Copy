
#include "Shared/Components/HitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"


UHitSphereComponent::UHitSphereComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}


void UHitSphereComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.RemoveDynamic(this, &UHitSphereComponent::OnHitSphereBeginOverlap);
	OnComponentBeginOverlap.AddDynamic(this, &UHitSphereComponent::OnHitSphereBeginOverlap);
}

void UHitSphereComponent::InitializeHitComp(FHitComp_Info HitInfo, FName TargetName)
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

void UHitSphereComponent::CollisionOn()
{
	if (bCollisionOn) return; 
	
	bCollisionOn = true;
	SetGenerateOverlapEvents(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpdateOverlaps();
}

void UHitSphereComponent::CollisionOff()
{	
	if (!bCollisionOn) return; 
	
	bCollisionOn = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}

void UHitSphereComponent::OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCollisionOn) return; 
	if (OtherActor == GetOwner()) return;
	if (!OtherActor->Tags.Contains(TargetTag)) return;
	
	if (AlreadyHitActors.Contains(OtherActor)) return;
	AlreadyHitActors.Add(OtherActor);
	
	UE_LOG(LogTemp, Log, TEXT("%s 와 충돌!"), *OtherActor->GetName());
	
	if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
	{
		FHitRequest Request(GetOwner(), OtherActor, Damage, SweepResult.ImpactPoint);
		CombatSystem->ProcessHit(Request);
	}
}

void UHitSphereComponent::OnHitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bAutoResetEndOverlap)
	{
		ClearHitRecords();
	}
}

