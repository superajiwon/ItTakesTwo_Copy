// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/Respawn_TargetPoint.h"


// Sets default values
ARespawn_TargetPoint::ARespawn_TargetPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARespawn_TargetPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawn_TargetPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

