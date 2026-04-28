// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Attacker.h"


// Sets default values
AMapObject_Attacker::AMapObject_Attacker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMapObject_Attacker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapObject_Attacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

