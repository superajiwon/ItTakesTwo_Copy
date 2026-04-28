// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_Attacker.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Attacker : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Attacker();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	
protected:
	float Damage{40.f};
	
	
};
