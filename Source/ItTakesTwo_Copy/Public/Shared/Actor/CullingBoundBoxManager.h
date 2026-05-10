// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CullingBoundBoxManager.generated.h"

class APlayerBase;
class ACullingBoundBox;

UCLASS()
class ITTAKESTWO_COPY_API ACullingBoundBoxManager : public AActor
{
	GENERATED_BODY()

public:
	ACullingBoundBoxManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void PlayerSectionChange(APlayerBase* PlayerActor, int32 NewSectionIndex);
	
	
	
private:
	void InitializeCulling();
	void GetAllSections();
	void ReCheckActiveSections();
	void ActivateStartSection();
	void AddRenderSection(ACullingBoundBox* BoundBox, TSet<int32>& NewSections);
	ACullingBoundBox* FindSectionByIndex(int32 SectionIndex) const;
	
	
	
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Culling|Section")
	int32 StartSectionIndex{0};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Culling|Section")
	TArray<TObjectPtr<ACullingBoundBox>> Sections;

private:
	UPROPERTY()
	TMap<TObjectPtr<AActor>, int32> PlayerSectionMap;
	TSet<int32> VisitedIndexes;
	TSet<int32> CurrentActiveSectionIndexes;
};
