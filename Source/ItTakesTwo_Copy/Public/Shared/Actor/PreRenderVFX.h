// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreRenderVFX.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreRenderFinished);


class UNiagaraSystem;
UCLASS()
class ITTAKESTWO_COPY_API APreRenderVFX : public AActor
{
	GENERATED_BODY()
public:
	APreRenderVFX();
	virtual void BeginPlay() override;
	
public:
	void StartPreRender();
	
private:
	void SpawnPreRenderBatch();
	
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPreRenderFinished OnPreRenderFinished;
	
	
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
	float PreRenderKeepAliveTime = 1.f;

	int32 CurrentPreRenderIndex = 0;

	FTimerHandle BatchTimerHandle;

};
