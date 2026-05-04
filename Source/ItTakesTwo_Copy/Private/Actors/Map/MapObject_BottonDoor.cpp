#include "Actors/Map/MapObject_BottonDoor.h"

#include "Actors/Map/MapObject_Attackable.h"

AMapObject_BottonDoor::AMapObject_BottonDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(true);
	MeshComp->SetupAttachment(RootComponent);
}

void AMapObject_BottonDoor::BeginPlay()
{
	Super::BeginPlay();

	AttackableCount = FMath::Clamp(AttackableCount, 1, 4);
}

void AMapObject_BottonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
		return;

	if (bOpened)
		return;

	if (!bActive)
	{
		CheckOpenCondition();
	}

	if (bActive)
	{
		OpenDoor(DeltaTime);
	}
}

void AMapObject_BottonDoor::CheckOpenCondition()
{
	int32 DestroyedCount = 0;

	if (AttackableCount >= 1)
	{
		if (!First_Attackable || !First_Attackable->IsDestroyed())
			return;

		++DestroyedCount;
	}

	if (AttackableCount >= 2)
	{
		if (!Second_Attackable || !Second_Attackable->IsDestroyed())
			return;

		++DestroyedCount;
	}

	if (AttackableCount >= 3)
	{
		if (!Third_Attackable || !Third_Attackable->IsDestroyed())
			return;

		++DestroyedCount;
	}

	if (AttackableCount >= 4)
	{
		if (!Fourth_Attackable || !Fourth_Attackable->IsDestroyed())
			return;

		++DestroyedCount;
	}

	if (DestroyedCount == AttackableCount)
	{
		bActive = true;
	}
}

void AMapObject_BottonDoor::OpenDoor(float DeltaTime)
{
	const float Direction = bRotation_AddPlus ? 1.f : -1.f;
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
		bActive = false;

		SetMapObjectState(EMapObjectState::Active);
		SetActorTickEnabled(false);
	}
}
