// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Map/MapObjectBase.h"
#include "MapObject_Crush.generated.h"

class UBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Crush : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Crush();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void ApplyDamage(float Damage);	
	void DestroyObject();
	void PlayDestroyEffect();
	
protected:
	UFUNCTION()
	void OnRep_CurrentHP();
	
	UFUNCTION()
	void OnRep_RegenTimer();
	
	UFUNCTION()
	void OnRep_Destroyed();
	
	UFUNCTION()
	void DestroyAfterDelay();
	
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	
protected:
	UPROPERTY(EditAnywhere, Category="ObjectSetting")
	float MaxHP = 100.f;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP{100.f};

	UPROPERTY(ReplicatedUsing=OnRep_Destroyed)
	bool bDestroyed = false;

	
};

