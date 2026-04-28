// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Attackable.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Net/UnrealNetwork.h"

AMapObject_Attackable::AMapObject_Attackable()
{
	PrimaryActorTick.bCanEverTick = true;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	// RootComponent = GeometryCollection;
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollection->SetCollisionProfileName(FName("MapObject"));
	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMapObject_Attackable::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CurrentHP = MaxHP;
	}
}

void AMapObject_Attackable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority() || bDestroyed)
		return;
	CurrentHP -= DeltaTime * 10.f; // 초당 10씩 감소
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);

	if (CurrentHP <= 0.f)
	{
		DestroyObject();
	}
}

void AMapObject_Attackable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMapObject_Attackable, CurrentHP);
	DOREPLIFETIME(AMapObject_Attackable, bDestroyed);
}

void AMapObject_Attackable::ApplyDamage(float Damage)
{
	if (!HasAuthority() || bDestroyed )
		return;
	
	CurrentHP -= Damage; 
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
	
	if (CurrentHP <= 0.f)
	{
		DestroyObject();
	}
	
	
}

void AMapObject_Attackable::DestroyObject()
{
	if (!HasAuthority() || bDestroyed)
		return;

	bDestroyed = true;
	CurrentHP = 0.f;

	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayDestroyEffect();

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AMapObject_Attackable::DestroyAfterDelay,
		5.f,
		false
	);
}

void AMapObject_Attackable::PlayDestroyEffect()
{
	if (!GeometryCollection)
		return;

	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollection->SetSimulatePhysics(true);

	const FVector Center = GeometryCollection->GetComponentLocation();
	const FVector Dir = FVector(1.f, 0.f, 0.5f).GetSafeNormal();

	// 주 방향 힘
	GeometryCollection->AddImpulse(
		Dir * 400.f, // 어디서 어느 방향으로 때렸는지 받아와야 할듯
		NAME_None,
		true
	);

	// 살짝 퍼지는 힘
	GeometryCollection->AddRadialImpulse(
		Center,
		600.f,
		200.f,
		ERadialImpulseFalloff::RIF_Linear,
		true
	);
}

void AMapObject_Attackable::OnRep_CurrentHP()
{
}

void AMapObject_Attackable::OnRep_RegenTimer()
{
}

void AMapObject_Attackable::OnRep_Destroyed()
{
	if (bDestroyed)
	{
		if (HitCollision)
		{
			HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		PlayDestroyEffect();
	}
}

void AMapObject_Attackable::DestroyAfterDelay()
{
	if (!HasAuthority())
		return;

	Destroy();
}

