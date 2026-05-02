// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Teleport_TargetPoint.h"


ATeleport_TargetPoint::ATeleport_TargetPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATeleport_TargetPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeleport_TargetPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

