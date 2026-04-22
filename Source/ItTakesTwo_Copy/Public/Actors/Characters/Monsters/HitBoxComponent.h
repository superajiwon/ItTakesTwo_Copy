// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Struct/HitComp_Info.h"
#include "HitBoxComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESTWO_COPY_API UHitBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UHitBoxComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	
public:
	void InitializeHitComp(FHitComp_Info HitInfo); // Extents, Location, TagName , ProfileName
	void CollisionOn();
	void CollisionOff();
	
public:
	bool GetCollisionState() const
	{
		return bCollisionOn;
	}

private:
	bool bCollisionOn{false};
	
	
	
};
