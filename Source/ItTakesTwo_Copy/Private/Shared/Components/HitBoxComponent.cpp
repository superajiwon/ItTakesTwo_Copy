// Fill out your copyright notice in the Description page of Project Settings.

#include "Shared/Components/HitBoxComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/UltimateComponent.h"
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
	
	OnComponentEndOverlap.RemoveDynamic(this, &UHitBoxComponent::OnHitBoxEndOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UHitBoxComponent::OnHitBoxEndOverlap);
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
	if (bCollisionOn) return; 
	
	bCollisionOn = true;
	SetGenerateOverlapEvents(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpdateOverlaps();
}

void UHitBoxComponent::CollisionOff()
{
	if (!bCollisionOn) return; 
	
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
	
	if (AlreadyHitActors.Contains(OtherActor)) return;
	AlreadyHitActors.Add(OtherActor);
	
	// UE_LOG(LogTemp, Log, TEXT("%s 와 충돌!"), *OtherActor->GetName());
	
	if (UCombatSystem* CombatSystem = GetWorld()->GetSubsystem<UCombatSystem>())
	{
		FVector HitLocation;
		if (bFromSweep) HitLocation = SweepResult.ImpactPoint;
		else			HitLocation = OtherActor->GetActorLocation();
		
		FHitRequest Request(GetOwner(), OtherActor, Damage, HitLocation);
		CombatSystem->ProcessHit(Request);
		
		// 서버에서만 Multicast 호출 → 모든 클라이언트에 HitVFX 전파
		if (GetOwner()->HasAuthority())
		{
			if (APlayerBase* Player = Cast<APlayerBase>(GetOwner()))
			{
				Player->Multicast_PlayHitVFX(HitLocation);
			}
		}
	}
}

void UHitBoxComponent::OnHitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bAutoResetEndOverlap)
	{
		ClearHitRecords();
	}
}

