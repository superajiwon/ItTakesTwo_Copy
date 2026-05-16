// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "Interfaces/Damagable.h"
#include "MapObject_Attackable.generated.h"

class AFloatingUIActor;
class UBoxComponent;
class UGeometryCollectionComponent;
UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Attackable : public AMapObjectBase, public IDamagable
{
	GENERATED_BODY()

public:
	AMapObject_Attackable();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Damage(float DamageAmount, AActor* Causer) override;
	
protected:
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
public:
	void ApplyDamage(float Damage);	
	void DestroyObject();
	void PlayDestroyEffect(const FVector& HitLocation,	const FVector& HitDirection);
	
public:
	bool IsDestroyed() const
	{
		return bDestroyed;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<AFloatingUIActor> FloatingUIClass;
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ShowDamageUI(float DamageAmount, FVector SpawnLocation);
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDestroySound(FName SoundId);
	
	
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

	UPROPERTY(Replicated)
	FVector_NetQuantize LastHitLocation;

	UPROPERTY(Replicated)
	FVector_NetQuantizeNormal LastHitDirection;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
	FName DestroySoundId{NAME_None};
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
