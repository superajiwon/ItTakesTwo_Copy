// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObjectBase.h"


AMapObjectBase::AMapObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;
	
	
	// MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// MeshComp->SetVisibility(true);
	// MeshComp->SetupAttachment(RootComponent);
	
}

void AMapObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AMapObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

