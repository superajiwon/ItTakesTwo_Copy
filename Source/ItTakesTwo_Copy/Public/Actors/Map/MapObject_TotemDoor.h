// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_TotemDoor.generated.h"

class AToyOgre_DropCollider;
class AMapObject_Totem;

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_TotemDoor : public AMapObjectBase
{
	GENERATED_BODY()
public:
	AMapObject_TotemDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	bool AllTotemCheckActivate();
	void OpenDoor(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDoorOpenSound();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TotemDoor|MapObject")
	TArray<TObjectPtr<AMapObject_Totem>> TotemList;
	
	UPROPERTY(EditAnywhere, Category="Drop")
	TObjectPtr<AToyOgre_DropCollider> DropCollider;
	
protected:
	UPROPERTY()
	bool bOpening{false};

	UPROPERTY()
	bool bOpened{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TotemDoor|MapObject")
	bool bRotation_Direction{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TotemDoor|MapObject")
	float DoorAngle{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TotemDoor|MapObject")
	float MaxDoorAngle{90.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TotemDoor|MapObject")
	float OpenSpeed{30.f};
	

};
             