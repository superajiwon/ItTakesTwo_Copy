// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFXObject.h"
#include "VFXExplosionObject.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AVFXExplosionObject : public AVFXObject
{
	GENERATED_BODY()

public:
	AVFXExplosionObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	void OnExplosionBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	
public:
	virtual void UseVFXObject(const FVFXSpawn_Info& SpawnInfo) override;
	virtual void FinishVFXObject() override;
	
	
	
};
