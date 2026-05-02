// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Subsystems/TargetPointSubsystem.h"

#include "Actors/Characters/Monsters/Teleport_TargetPoint.h"
#include "Actors/Characters/Players/Respawn_TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void UTargetPointSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	// 텔레포트용 액터 위치 받아서 저장
	TArray<AActor*> TeleportPoints;
	UGameplayStatics::GetAllActorsOfClass(&InWorld,
		ATeleport_TargetPoint::StaticClass(), TeleportPoints);

	for (auto& Point : TeleportPoints)
	{
		TeleportPointList.Add(Cast<ATeleport_TargetPoint>(Point));
	}
	
	
	// 리스폰 위치 받아서 저장
	TArray<AActor*> RespawnPoints;
	UGameplayStatics::GetAllActorsOfClass(&InWorld,
		ARespawn_TargetPoint::StaticClass(), RespawnPoints);

	for (auto& Point : RespawnPoints)
	{
		RespawnPointList.Add(Cast<ARespawn_TargetPoint>(Point));
	}
}

TArray<TObjectPtr<ATeleport_TargetPoint>> UTargetPointSubsystem::GetTeleportTransform()
{
	
	return TeleportPointList;
}

TArray<TObjectPtr<ARespawn_TargetPoint>> UTargetPointSubsystem::GetRespawnTransform()
{
	return RespawnPointList;
}

