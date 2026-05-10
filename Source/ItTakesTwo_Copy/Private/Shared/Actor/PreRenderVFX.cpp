// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/PreRenderVFX.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Shared/VFXObjectPoolSubsystem.h"


APreRenderVFX::APreRenderVFX()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
}

void APreRenderVFX::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		PreRenderTimerHandle,
		this,
		&APreRenderVFX::StartPreRender,
		PreRenderDelay,
		false
	);
}

void APreRenderVFX::StartPreRender()
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	if (UVFXObjectPoolSubsystem* Pool = World->GetSubsystem<UVFXObjectPoolSubsystem>())
	{
		Pool->PreRenderVFXPool();
	}

	CurrentPreRenderIndex = 0;
	SpawnPreRenderBatch();
}


void APreRenderVFX::SpawnPreRenderBatch()
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	const int32 EndIndex = FMath::Min(
		CurrentPreRenderIndex + PreRenderBatchSize,
		PreNiagaraAssets.Num()
	);

	for (int32 i = CurrentPreRenderIndex; i < EndIndex; ++i)
	{
		UNiagaraSystem* NiagaraAsset = PreNiagaraAssets[i];
		if (!NiagaraAsset)
			continue;

		const FVector SpawnLocation =
			GetActorLocation() + FVector(0.f, i * 100.f, 0.f);

		UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			NiagaraAsset,
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector(1.f),
			true,
			true
		);

		if (Comp)
		{
			FTimerHandle DeactivateTimerHandle;
			World->GetTimerManager().SetTimer(
				DeactivateTimerHandle,
				[Comp]()
				{
					if (IsValid(Comp))
					{
						Comp->Deactivate();
					}
				},
				PreRenderKeepAliveTime,
				false
			);
		}
	}

	CurrentPreRenderIndex = EndIndex;

	if (CurrentPreRenderIndex < PreNiagaraAssets.Num())
	{
		World->GetTimerManager().SetTimer(
			BatchTimerHandle,
			this,
			&APreRenderVFX::SpawnPreRenderBatch,
			PreRenderBatchInterval,
			false
		);
	}
}