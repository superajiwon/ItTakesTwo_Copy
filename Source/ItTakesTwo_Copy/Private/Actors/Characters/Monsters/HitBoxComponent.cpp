// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/HitBoxComponent.h"

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
	if (!bCollisionOn)
		return; 
	
	if (OtherActor == GetOwner())
		return;
	
	// 충돌 액터 찾으면
	// 인터페이스 호출해서 데미지 주거나 어떤 공통된 로직이 있으면 좋을듯함
	if (OtherActor->Tags.Contains(TargetTag)) // 이런식으로..? 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 와 충돌!"), *OtherActor->GetName());
		// 충돌하면 충돌한 상대 무적상태 돌입
	}
}

