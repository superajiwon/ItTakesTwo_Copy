// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapObjectBase.generated.h"

UENUM(BlueprintType)
enum class EMapObjectState : uint8
{
	Hidden,
	BeforePressed,
	AfterPressed,
	Active,
	Destroyed
};

UCLASS()
class ITTAKESTWO_COPY_API AMapObjectBase : public AActor
{
	GENERATED_BODY()

public:
	AMapObjectBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	EMapObjectState GetMapObjectState() const
	{
		return CurrentState;
	}
	
public:
	void SetMapObjectState(EMapObjectState NewState)
	{
		CurrentState = NewState;
	}
	
	
protected:
	UPROPERTY(Replicated)
	EMapObjectState CurrentState{EMapObjectState::Hidden};
	
	

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComp;
	
};
