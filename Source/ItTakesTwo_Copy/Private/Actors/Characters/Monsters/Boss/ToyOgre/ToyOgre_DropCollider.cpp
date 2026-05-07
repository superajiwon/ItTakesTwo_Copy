// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_DropCollider.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Components/SphereComponent.h"


AToyOgre_DropCollider::AToyOgre_DropCollider()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereCollider"));
	SphereComp->SetSphereRadius(150.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = SphereComp;
		
}

void AToyOgre_DropCollider::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	if (HasAuthority() && SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AToyOgre_DropCollider::OnDropBeginOverlap);
	}
}

void AToyOgre_DropCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bActive)
		return;
	DrawDebugSphere(
	GetWorld(),
	GetActorLocation(),
	SphereComp->GetScaledSphereRadius(),
	32,
	FColor::Red,
	false,
	0.f,
	0,
	2.f
);
}

void AToyOgre_DropCollider::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// if (!HasAuthority())
	// 	return;
	//
	// if (!bActive)
	// 	return;
	//
	// if (AToyOgre_Monster* Ogre = Cast<AToyOgre_Monster>(OtherActor))
	// {
	// 	Ogre->GetStateMachine()->ChangeState(Ogre->HoleEnterStateClass);  // 이거 Hole_Enter로 바꾸기
	// 	Destroy();
	// }
}

void AToyOgre_DropCollider::OnDropBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	if (!bActive)
		return;
	
	if (AToyOgre_Monster* Ogre = Cast<AToyOgre_Monster>(OtherActor))
	{
		Ogre->GetStateMachine()->ChangeState(Ogre->HoleEnterStateClass);  // 이거 Hole_Enter로 바꾸기
		Destroy();
	}
}

void AToyOgre_DropCollider::BeginActive()
{
	bActive = true;

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	if (SphereComp)
	{
		SphereComp->SetGenerateOverlapEvents(true);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComp->UpdateOverlaps();
		
	}
}

