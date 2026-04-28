// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "DotHitSphereComponent.generated.h"

struct FHitComp_Info;
class UCombatSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESTWO_COPY_API UDotHitSphereComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDotHitSphereComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void InitializeHitComp(FHitComp_Info HitInfo, FName TargetName = NAME_None); // Extents, Location, TagName, ProfileName
	void CollisionOn();
	void CollisionOff();
	
	bool GetCollisionState() const { return bCollisionOn; }
	FName GetTargetTag() const { return TargetTag; }

	void SetDamage(int InDamage) { Damage = InDamage; }
	int GetDamage() const { return Damage; }
	
	// DoT(지속 데미지) 전용 설정 함수!
	void SetDotInterval(float InInterval) { DotInterval = InInterval; }
	
private:
	bool bCollisionOn{false};
	FName TargetTag{NAME_None};
	int Damage{0};
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float DotInterval{0.55f};
	
	float AccumulatedTime{0.0f}; // 누적 시간
};
