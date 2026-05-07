// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Totem.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


AMapObject_Totem::AMapObject_Totem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(true);
	MeshComp->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetGenerateOverlapEvents(true);

}

void AMapObject_Totem::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetMapObjectState(EMapObjectState::BeforePressed);
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMapObject_Totem::OnActivationBeginOverlap);
	}
	
}

void AMapObject_Totem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapObject_Totem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMapObject_Totem, bActivated);
}

void AMapObject_Totem::OnActivationBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || bActivated)
		return;

	if (!Cast<AToyOgre_Monster>(OtherActor))
		return;

	ActivateTotem();
}

void AMapObject_Totem::OnRep_Activated()
{
}

void AMapObject_Totem::ActivateTotem()
{
	if (bActivated)
		return;

	bActivated = true;
	SetMapObjectState(EMapObjectState::Active);

	OnRep_Activated();
}

