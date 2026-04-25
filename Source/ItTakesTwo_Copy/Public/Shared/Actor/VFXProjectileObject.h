// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFXObject.h"
#include "VFXProjectileObject.generated.h"


UCLASS()
class ITTAKESTWO_COPY_API AVFXProjectileObject : public AVFXObject
{
	GENERATED_BODY()

public:
	AVFXProjectileObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	
public:
	virtual void UseVFXObject(const FVFXSpawn_Info& SpawnInfo) override;
	virtual void FinishVFXObject() override;
	
private:
	
protected:
	UFUNCTION()
	void OnProjectileBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

};
