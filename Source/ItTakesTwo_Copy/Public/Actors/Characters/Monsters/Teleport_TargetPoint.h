// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Teleport_TargetPoint.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API ATeleport_TargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ATeleport_TargetPoint();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	int32 GetIndex() const
	{
		return PointIndex;
	}	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Teleport")
	int32 PointIndex{};
	
	
};
