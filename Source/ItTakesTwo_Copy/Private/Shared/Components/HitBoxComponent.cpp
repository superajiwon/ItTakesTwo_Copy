// Fill out your copyright notice in the Description page of Project Settings.

#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Shared/Struct/HitRequest.h"
#include "Shared/Subsystems/CombatSystem.h"

UHitBoxComponent::UHitBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}

void UHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.RemoveDynamic(this, &UHitBoxComponent::OnHitBoxBeginOverlap);
	OnComponentBeginOverlap.AddDynamic(this, &UHitBoxComponent::OnHitBoxBeginOverlap);
}

void UHitBoxComponent::InitializeHitComp(FHitComp_Info HitInfo, FName TargetName)
{
	ComponentTags.Reset();
	ComponentTags.Add(HitInfo.HitTagName);
	SetBoxExtent(HitInfo.HitBoxExtents);
	SetRelativeLocation(HitInfo.HitBoxLocation);
	SetCollisionProfileName(HitInfo.CollisionProfileName);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
	TargetTag = TargetName;
}

void UHitBoxComponent::CollisionOn()
{
	bCollisionOn = true;
	SetGenerateOverlapEvents(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpdateOverlaps();
}

void UHitBoxComponent::CollisionOff()
{
	bCollisionOn = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}

void UHitBoxComponent::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCollisionOn) return; 
	if (OtherActor == GetOwner()) return;
	
	if (!OtherActor->Tags.Contains(TargetTag)) return;
	
	if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
	{
		FHitRequest Request(GetOwner(), OtherActor, Damage, SweepResult.ImpactPoint);
		CombatSystem->ProcessHit(Request);
	}
}

