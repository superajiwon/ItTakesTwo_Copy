#include "Actors/Map/MapObject_BottonDoor.h"

#include "Actors/Characters/Monsters/Boss/BossBase.h"
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

	// bOpened 체크를 CrusherMove 이후로 분리
	if (!bCrusherMoveStarted && !bOpened && CanStartCrusherMove())
	{
		StartCrusherMove();
		bActive = true;
	}

	if (!bOpened && bActive)
	{
		OpenDoor(DeltaTime);
	}

	// bOpened 여부와 관계없이 CrusherMove는 계속 실행
	if (bCrusherMoveStarted)
	{
		MoveCrusherAlongSocket(DeltaTime);
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
	}
}

bool AMapObject_BottonDoor::AreAllAttackablesDestroyed() const
{
	if (AttackableCount >= 1)
	{
		if (!First_Attackable || !First_Attackable->IsDestroyed())
			return false;
	}

	if (AttackableCount >= 2)
	{
		if (!Second_Attackable || !Second_Attackable->IsDestroyed())
			return false;
	}

	if (AttackableCount >= 3)
	{
		if (!Third_Attackable || !Third_Attackable->IsDestroyed())
			return false;
	}

	if (AttackableCount >= 4)
	{
		if (!Fourth_Attackable || !Fourth_Attackable->IsDestroyed())
			return false;
	}

	return true;
}

bool AMapObject_BottonDoor::CanStartCrusherMove() const
{
	if (!MeshComp || !RelativeBoss)
		return false;

	if (!AreAllAttackablesDestroyed())
		return false;

	const FVector DropCheckSocketWorldLocation =
		MeshComp->GetSocketLocation(DropCheckSocketName);

	return RelativeBoss->GetActorLocation().X <= DropCheckSocketWorldLocation.X;
}


void AMapObject_BottonDoor::StartCrusherMove()
{
	if (!MeshComp || !RelativeBoss)
		return;

	bCrusherMoveStarted = true;
	CrusherCurrentMoveX = 0.f;
	CrusherStartRotation = FRotator(0.f,180.f,0.f);

	// 소켓은 위치만 사용
	const FVector SocketWorldLocation =
		MeshComp->GetSocketLocation(StartDropSocketName);

	// 회전 Transform 적용 없이 월드 Z 오프셋만 적용
	const FVector StartLocation =
		SocketWorldLocation + FVector(0.f, 0.f, CrusherZOffset);

	RelativeBoss->SetActorLocation(StartLocation, false);
}
void AMapObject_BottonDoor::MoveCrusherAlongSocket(float DeltaTime)
{
	if (!MeshComp || !RelativeBoss)
		return;

	// 양수로 누적 (뒤로 가는 실제 방향은 아래에서 처리)
	CrusherCurrentMoveX += CrusherMoveSpeedX * DeltaTime;

	const FVector SocketWorldLocation =
		MeshComp->GetSocketLocation(StartDropSocketName);

	// ForwardVector 대신 -ForwardVector 또는 명시적 월드 방향 사용
	const FVector MoveDirection = -RelativeBoss->GetActorForwardVector();

	const FVector TargetLocation =
		SocketWorldLocation
		+ MoveDirection * CrusherCurrentMoveX
		+ FVector(0.f, 0.f, CrusherZOffset);

	RelativeBoss->SetActorLocation(TargetLocation, false);

	FRotator CrusherRotation = CrusherStartRotation;
	CrusherRotation.Pitch = CrusherStartRotation.Pitch + (-1 * DoorAngle);

	if (bMatchCrusherPitchToDoor)
	{
		RelativeBoss->SetActorRotation(CrusherRotation);
	}
}
