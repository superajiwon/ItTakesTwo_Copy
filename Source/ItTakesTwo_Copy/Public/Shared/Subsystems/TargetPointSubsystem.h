// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TargetPointSubsystem.generated.h"


class ARespawn_TargetPoint;
class ATeleport_TargetPoint;

UCLASS()
class ITTAKESTWO_COPY_API UTargetPointSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	
public:
	TArray<TObjectPtr<ATeleport_TargetPoint>> GetTeleportTransform();
	TArray<TObjectPtr<ARespawn_TargetPoint>> GetRespawnTransform();
	
	
private:
	UPROPERTY()
	TArray<TObjectPtr<ATeleport_TargetPoint>> TeleportPointList;
	
	UPROPERTY()
	TArray<TObjectPtr<ARespawn_TargetPoint>> RespawnPointList;
	
	
};
