// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToyOgre_DropCollider.generated.h"

class USphereComponent;
UCLASS()
class ITTAKESTWO_COPY_API AToyOgre_DropCollider : public AActor
{
	GENERATED_BODY()

public:
	AToyOgre_DropCollider();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


private:
	UFUNCTION()
	void OnDropBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
public:
	void BeginActive();
public:
	bool IsActive() const
	{
		return bActive;
	}
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;
	
private:
	UPROPERTY()
	bool bActive{false};
};
