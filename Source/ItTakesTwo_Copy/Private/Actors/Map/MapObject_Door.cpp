// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Door.h"

#include "Actors/Map/MapObject_Button.h"


AMapObject_Door::AMapObject_Door()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(true);
	MeshComp->SetupAttachment(RootComponent);
}

void AMapObject_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapObject_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
		return;
	
	if (bActive)
	{
		OpenDoor(DeltaTime);
		return;
	}
	
	int32 PressedCount{0};
	for (int i = 0; i < ButtonCount; i++)
	{
		switch (i)
		{
			case 0:
			{
				if (First_Button->GetMapObjectState() == EMapObjectState::AfterPressed)
				{
					PressedCount++;
				}
				break;
			}
			case 1:
			{
				if (Second_Button->GetMapObjectState() == EMapObjectState::AfterPressed)
				{
					PressedCount++;
				}
				break;
			}
			default: 
				break;
		}
		
	}
	if (PressedCount == ButtonCount)
	{
		bActive = true;
	}
}

void AMapObject_Door::OpenDoor(float DeltaTime)
{
	const float Direction = bRotation_AddPlus ? 1.f : -1.f;
	const float TargetAngle = Direction * MaxDoorAngle;
	
	DoorAngle = FMath::FInterpConstantTo(
		DoorAngle,
		MaxDoorAngle,
		DeltaTime,
		30.f
	);

	FRotator Rotation = GetActorRotation();
	Rotation.Yaw = DoorAngle;
	SetActorRotation(Rotation);
	
	if (FMath::IsNearlyEqual(DoorAngle, TargetAngle, 0.1f))
	{
		DoorAngle = TargetAngle;
		SetActorTickEnabled(false);
	}
}



