// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Crush.h"

#include "Actors/Map/MapObject_Attackable.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Net/UnrealNetwork.h"


AMapObject_Crush::AMapObject_Crush()
{
	PrimaryActorTick.bCanEverTick = true;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollection->SetCollisionProfileName(FName("MapObject"));
	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	Tags.Add(FName("Monster"));
}

void AMapObject_Crush::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CurrentHP = MaxHP;
	}
}

void AMapObject_Crush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority() || bDestroyed)
		return;

	if (CurrentHP <= 0.f)
	{
		DestroyObject();
	}
}

void AMapObject_Crush::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMapObject_Crush, CurrentHP);
	DOREPLIFETIME(AMapObject_Crush, bDestroyed);
}

void AMapObject_Crush::ApplyDamage(float Damage)
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

void AMapObject_Crush::DestroyObject()
{
	if (!HasAuthority() || bDestroyed)
		return;

	bDestroyed = true;
	CurrentHP = 0.f;

	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (GeometryCollection)
	{
		GeometryCollection->SetCollisionProfileName(FName("MapObject_Crush"));
	}
	PlayDestroyEffect();

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AMapObject_Crush::DestroyAfterDelay,
		5.f,
		false
	);
	
}

void AMapObject_Crush::PlayDestroyEffect()
{
	if (!GeometryCollection)
		return;

	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollection->SetSimulatePhysics(true);

	const FVector Center = GeometryCollection->GetComponentLocation();
	const FVector Dir = FVector(0.f, 0.f, -0.5f).GetSafeNormal();

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

void AMapObject_Crush::OnRep_CurrentHP()
{
	
	
}

void AMapObject_Crush::OnRep_RegenTimer()
{
}

void AMapObject_Crush::OnRep_Destroyed()
{
	if (bDestroyed)
	{
		if (HitCollision)
		{
			HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (GeometryCollection)
		{
			GeometryCollection->SetCollisionProfileName(FName("MapObject_Crush"));
		}
		PlayDestroyEffect();
	}
}


void AMapObject_Crush::DestroyAfterDelay()
{
	if (!HasAuthority())
		return;

	Destroy();
}

