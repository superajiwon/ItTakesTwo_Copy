// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitSphereComponent.generated.h"

struct FHitComp_Info;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESTWO_COPY_API UHitSphereComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UHitSphereComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHitSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);
	
public:
	void InitializeHitComp(FHitComp_Info HitInfo, FName TargetName = NAME_None); // Extents, Location, TagName , ProfileName
	void CollisionOn();
	void CollisionOff();
	
public:
	bool GetCollisionState() const
	{
		return bCollisionOn;
	}

private:
	bool bCollisionOn{false};
	
	FName TargetTag{NAME_None};
};
