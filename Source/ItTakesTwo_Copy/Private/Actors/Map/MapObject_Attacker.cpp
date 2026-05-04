// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Attacker.h"

#include "Components/SplineComponent.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"


AMapObject_Attacker::AMapObject_Attacker()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("HitBoxComponent"));
	HitBoxComponent->SetupAttachment(RootComponent);
	HitBoxComponent->SetDamage(Damage);
	
}

void AMapObject_Attacker::BeginPlay()
{
	Super::BeginPlay();
	if (SplineActor)
	{
		SplineComponent = SplineActor->FindComponentByClass<USplineComponent>();
	}

	if (HasAuthority() && HitBoxComponent)
	{
		FHitComp_Info HitInfo(
			FName(TEXT("MapObject")),
			FName(TEXT("MonsterWeapon")),
			FVector::ZeroVector,
			FVector(100.f, 100.f, 100.f)
		);

		HitBoxComponent->InitializeHitComp(HitInfo, FName(TEXT("Player")));
		HitBoxComponent->SetDamage(static_cast<int32>(Damage));
		HitBoxComponent->CollisionOn();
	}
	
	if (!SplineComponent)
	{
		SetActorTickEnabled(false);
		return;
	}

	CurrentDistance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(SplineComponent->FindInputKeyClosestToWorldLocation(GetActorLocation()));

	
}

void AMapObject_Attacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
		return;

	if (!SplineComponent)
		return;
	
	const float SplineLength = SplineComponent->GetSplineLength();
	if (SplineLength <= 0.f)
		return;

	CurrentDistance += Speed * DeltaTime * MoveDirection;
	if (CurrentDistance >= SplineLength)
	{
		CurrentDistance = SplineLength;
		MoveDirection = -1;
	}
	else if (CurrentDistance <= 0.f)
	{
		CurrentDistance = 0.f;
		MoveDirection = 1;
	}
	const FVector NewLocation =
	SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	SetActorLocation(NewLocation);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(Rotation );
}

