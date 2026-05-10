// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreRenderVFX.generated.h"

class UNiagaraSystem;

UCLASS()
class ITTAKESTWO_COPY_API APreRenderVFX : public AActor
{
	GENERATED_BODY()
public:
	APreRenderVFX();
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, Category="VFX|PreNiagara")
	TArray<TObjectPtr<UNiagaraSystem>> PreNiagaraAssets;

	UPROPERTY(EditAnywhere, Category="VFX|PreNiagara")
	float PreRenderDelay = 0.3f;
	
	FTimerHandle PreRenderTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="VFX|PreNiagara")
	int32 PreRenderBatchSize = 5;

	UPROPERTY(EditAnywhere, Category="VFX|PreNiagara")
	float PreRenderBatchInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category="VFX|PreNiagara")
	float PreRenderKeepAliveTime = 0.3f;

	int32 CurrentPreRenderIndex = 0;

	FTimerHandle BatchTimerHandle;
private:
	void StartPreRender();
	void SpawnPreRenderBatch();
};
