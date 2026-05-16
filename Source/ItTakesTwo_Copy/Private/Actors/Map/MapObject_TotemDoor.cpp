// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_TotemDoor.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_DropCollider.h"
#include "Actors/Map/MapObject_Totem.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"


AMapObject_TotemDoor::AMapObject_TotemDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(true);
	MeshComp->SetupAttachment(RootComponent);
	
}

void AMapObject_TotemDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetMapObjectState(EMapObjectState::BeforePressed);
	}
}

void AMapObject_TotemDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority() || bOpened)
		return;

	if (!bOpening && AllTotemCheckActivate())
	{
		bOpening = true;
		Multicast_PlayDoorOpenSound();
	}
	if (bOpening)
		OpenDoor(DeltaTime);
	
	
}

bool AMapObject_TotemDoor::AllTotemCheckActivate()
{
	if (TotemList.Num() == 0)
		return false;
	
	for (const TObjectPtr<AMapObject_Totem>& Totem : TotemList)
	{
		if (!Totem || Totem->IsActivated() == false)
			return false;
	}
	
	return true;
}

void AMapObject_TotemDoor::OpenDoor(float DeltaTime)
{
	const float Direction = bRotation_Direction ? 1.f : -1.f;
	const float TargetAngle = Direction * MaxDoorAngle;

	DoorAngle = FMath::FInterpConstantTo(
		DoorAngle,
		TargetAngle,
		DeltaTime,
		OpenSpeed
	);

	FRotator Rotation = GetActorRotation();
	Rotation.Pitch = DoorAngle;
	SetActorRotation(Rotation);

	if (FMath::IsNearlyEqual(DoorAngle, TargetAngle, 0.1f))
	{
		DoorAngle = TargetAngle;
		Rotation.Pitch = DoorAngle;
		SetActorRotation(Rotation);

		bOpened = true;
		bOpening = false;

		SetMapObjectState(EMapObjectState::Active);
		if (DropCollider)	
			DropCollider->BeginActive();
		
		
		SetActorTickEnabled(false);
	}
}

void AMapObject_TotemDoor::Multicast_PlayDoorOpenSound_Implementation()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager =
			GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX2D(TEXT("Ogre_DoorOpen"));
		}
	}
}

