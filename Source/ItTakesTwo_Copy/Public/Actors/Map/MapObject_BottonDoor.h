// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_BottonDoor.generated.h"

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
	void CheckOpenCondition();
	void OpenDoor(float DeltaTime);

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

	UPROPERTY()
	bool bActive{false};

	UPROPERTY()
	bool bOpened{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	bool bRotation_AddPlus{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float DoorAngle{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float MaxDoorAngle{90.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MapObject_Door")
	float OpenSpeed{30.f};
	
};
