// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_Door.generated.h"

class AMapObject_Button;

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Door : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Door();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	void OpenDoor(float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	int32 ButtonCount{1};
	
	UPROPERTY()
	bool bActive{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	bool bRotation_AddPlus{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	float DoorAngle{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	float MaxDoorAngle{90.f};
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	TObjectPtr<AMapObject_Button> First_Button;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	TObjectPtr<AMapObject_Button> Second_Button;
	
};
