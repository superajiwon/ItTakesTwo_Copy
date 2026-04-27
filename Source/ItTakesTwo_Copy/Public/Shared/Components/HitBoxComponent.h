// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HitBoxComponent.generated.h"

struct FHitComp_Info;
class UCombatSystem;

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
	void InitializeHitComp(FHitComp_Info HitInfo, FName TargetName = NAME_None); // Extents, Location, TagName, ProfileName
	void CollisionOn();
	void CollisionOff();
	
public:
	bool GetCollisionState() const
	{
		return bCollisionOn;
	}
	
	FName GetTargetTag() const
	{
		return TargetTag;
	}

private:
	bool bCollisionOn{false};

	FName TargetTag{NAME_None};
	
	/** 이 히트박스로 가할 데미지 수치 (InitializeHitComp 또는 외부에서 세팅) */
	int Damage{0};

public:
	void SetDamage(int InDamage) { Damage = InDamage; }
	int GetDamage() const { return Damage; }
};
