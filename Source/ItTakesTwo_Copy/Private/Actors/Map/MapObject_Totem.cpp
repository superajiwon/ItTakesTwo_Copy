// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Totem.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"


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
	if (!bActivated)
		return;
	PlayChainSequence();
	auto DeactivateNiagara = [](ANiagaraActor* NiagaraActor)
	{
		if (!NiagaraActor)
			return;

		UNiagaraComponent* NiagaraComp = NiagaraActor->GetNiagaraComponent();
		if (!NiagaraComp)
			return;

		NiagaraComp->Deactivate();
	//	NiagaraComp->SetVisibility(false, true);
	};

	DeactivateNiagara(PlacedNiagaraActor1);
	DeactivateNiagara(PlacedNiagaraActor2);
	DeactivateNiagara(PlacedNiagaraActor3);
	DeactivateNiagara(PlacedNiagaraActor4);
}

void AMapObject_Totem::ActivateTotem()
{
	if (bActivated)
		return;

	
	bActivated = true;
	SetMapObjectState(EMapObjectState::Active);

	OnRep_Activated();
}

void AMapObject_Totem::PlayChainSequence()
{
	if (!ChainSequenceActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Totem] ChainSequenceActor is null: %s"), *GetName());
		return;
	}

	ULevelSequencePlayer* SequencePlayer = ChainSequenceActor->GetSequencePlayer();
	if (!SequencePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Totem] SequencePlayer is null: %s"), *GetName());
		return;
	}

	SequencePlayer->Play();
}

