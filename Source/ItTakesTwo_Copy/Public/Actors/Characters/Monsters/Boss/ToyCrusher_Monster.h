// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBase.h"
#include "ToyCrusher_Monster.generated.h"

class UBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API AToyCrusher_Monster : public ABossBase
{
	GENERATED_BODY()

public:
	AToyCrusher_Monster();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
public:
	virtual void Tick(float DeltaTime) override;

	
private:
	UPROPERTY()
	UBoxComponent* BoxComponent;
};
