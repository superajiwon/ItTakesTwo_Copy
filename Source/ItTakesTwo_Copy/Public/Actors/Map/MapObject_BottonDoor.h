#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_BottonDoor.generated.h"

class AMapObject_Attackable;
class ABossBase;

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_BottonDoor : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_BottonDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void OpenDoor(float DeltaTime);

	bool AreAllAttackablesDestroyed() const;
	bool CanStartCrusherMove() const;
	void StartCrusherMove();
	void MoveCrusherAlongSocket(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Attackable")
	int32 AttackableCount{1};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Attackable")
	TObjectPtr<AMapObject_Attackable> First_Attackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Attackable")
	TObjectPtr<AMapObject_Attackable> Second_Attackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Attackable")
	TObjectPtr<AMapObject_Attackable> Third_Attackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Attackable")
	TObjectPtr<AMapObject_Attackable> Fourth_Attackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	TObjectPtr<ABossBase> RelativeBoss;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	FName StartDropSocketName = TEXT("StartDropPos");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	FName DropCheckSocketName = TEXT("DropPos");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	float CrusherMoveSpeedX = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	float CrusherMaxMoveX = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	float CrusherZOffset = 1010.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	bool bMatchCrusherPitchToDoor = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	bool bReverseCrusherFacing = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crusher")
	FRotator CrusherRotationOffset = FRotator::ZeroRotator;

	UPROPERTY()
	FRotator CrusherStartRotation = FRotator::ZeroRotator;

	UPROPERTY()
	float CrusherCurrentMoveX = 0.f;

	UPROPERTY()
	bool bCrusherMoveStarted = false;

	UPROPERTY()
	bool bActive{false};

	UPROPERTY()
	bool bOpened{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	bool bRotation_AddPlus{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float DoorAngle{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float MaxDoorAngle{45.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float OpenSpeed{30.f};
};
