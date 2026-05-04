// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Door.h"

#include "Actors/Map/MapObject_Button.h"
#include "Components/BoxComponent.h"

AMapObject_Door::AMapObject_Door()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMapObject_Door::BeginPlay()
{
	Super::BeginPlay();

}
void AMapObject_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() || bDestroyed)
		return;

	if (bActive)
	{
		OpenDoor(DeltaTime);
		return;
	}

	int32 PressedCount = 0;

	if (ButtonCount >= 1 && First_Button &&
		First_Button->GetMapObjectState() == EMapObjectState::AfterPressed)
	{
		++PressedCount;
	}

	if (ButtonCount >= 2 && Second_Button &&
		Second_Button->GetMapObjectState() == EMapObjectState::AfterPressed)
	{
		++PressedCount;
	}

	if (PressedCount == ButtonCount)
	{
		bActive = true;
	}
}


void AMapObject_Door::Damage(float DamageAmount, AActor* Causer)
{
	if (!HasAuthority())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("[Door Damage] %s Damage=%.1f Causer=%s Auth=%d"),
		*GetName(),
		DamageAmount,
		*GetNameSafe(Causer),
		HasAuthority());
	
	Super::Damage(DamageAmount, Causer);
}

void AMapObject_Door::OpenDoor(float DeltaTime)
{
	const float Direction = bRotation_AddPlus ? 1.f : -1.f;
	const float TargetAngle = Direction * MaxDoorAngle;

	DoorAngle = FMath::FInterpConstantTo(
		DoorAngle,
		TargetAngle,
		DeltaTime,
		30.f
	);

	FRotator Rotation = GetActorRotation();
	Rotation.Yaw = DoorAngle;
	SetActorRotation(Rotation);

	if (FMath::IsNearlyEqual(DoorAngle, TargetAngle, 0.1f))
	{
		DoorAngle = TargetAngle;
		Rotation.Yaw = DoorAngle;
		SetActorRotation(Rotation);
		SetMapObjectState(EMapObjectState::Active);
	}
}



