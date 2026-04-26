// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Struct/FVFXSpawn_Info.h"
#include "Shared/Struct/VFXSpawnRep_Info.h"
#include "VFXObject.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;
UCLASS()
class ITTAKESTWO_COPY_API AVFXObject : public AActor
{
	GENERATED_BODY()

public:
	AVFXObject();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnVFXSystemFinished(UNiagaraComponent* FinishedComponent);
	
	
public:
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void UseVFXObject(const FVFXSpawn_Info& SpawnInfo);
	virtual void FinishVFXObject();
	virtual void CheckLifeTime(float DeltaTime);
	virtual void CheckDistance(float DeltaTime);
public:
	bool IsUsing() const
	{
		return bUsing;
	}
	

private:
	void UseCollision();
	void FinishCollision();
	void Initialize_ForStart();
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_VFXRepState();

	void SetVFXRepStateFromSpawnInfo(const FVFXSpawn_Info& SpawnInfo);
	void ApplyVisualState(const FVFXSpawn_Info& SpawnInfo);
	void FinishVisualState();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_VFXRepState)
	FVFXSpawnRep_Info VFXRepState;
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFXComponent;

	UPROPERTY()
	FVFXSpawn_Info VFXInfo;
	
	
protected:
	UPROPERTY()
	bool bUsing{false};

	UPROPERTY()
	float CurrentLifeTime{1.f};

	UPROPERTY()
	float CurrentLifeDistance{1.f};
	
};

inline void AVFXObject::Initialize_ForStart()
{
	bUsing = true;
	CurrentLifeTime = 0.f;
	CurrentLifeDistance = 0.f;
}
