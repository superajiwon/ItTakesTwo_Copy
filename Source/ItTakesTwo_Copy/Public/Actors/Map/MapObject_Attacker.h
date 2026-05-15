// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "MapObject_Attacker.generated.h"




class USplineComponent;
class UHitBoxComponent;
UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Attacker : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Attacker();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	void StartLoopSound();
	void StopLoopSound();


public:
	UFUNCTION(BlueprintCallable)
	void SetAttackerActive(bool bActive);
	
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spline")
	TObjectPtr<AActor> SplineActor;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spline")
	float Speed{300.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	float RotationSpeed {600.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	float Damage {40.f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	TObjectPtr<UHitBoxComponent> HitBoxComponent;
	
	float CurrentDistance = 0.f;
	int32 MoveDirection = 1;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
	TObjectPtr<USoundBase> LoopSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
	float SoundFadeOutTime = 0.25f;
};
