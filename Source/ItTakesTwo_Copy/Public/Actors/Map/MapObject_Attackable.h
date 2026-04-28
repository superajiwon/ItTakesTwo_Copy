// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_Attackable.generated.h"

class UBoxComponent;
class UGeometryCollectionComponent;
UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Attackable : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Attackable();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void ApplyDamage(float Damage);	
	void DestroyObject();
	void PlayDestroyEffect();
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	
protected:
	UPROPERTY(EditAnywhere, Category="ObjectSetting")
	float MaxHP = 100.f;
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP{100.f};

	UPROPERTY(ReplicatedUsing=OnRep_Destroyed)
	bool bDestroyed = false;

	
	
protected:
	UFUNCTION()
	void OnRep_CurrentHP();
	
	UFUNCTION()
	void OnRep_RegenTimer();
	
	UFUNCTION()
	void OnRep_Destroyed();
	
	UFUNCTION()
	void DestroyAfterDelay();
};
