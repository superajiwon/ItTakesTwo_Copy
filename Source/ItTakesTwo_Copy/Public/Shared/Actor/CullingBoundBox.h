// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CullingBoundBox.generated.h"

class UBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API ACullingBoundBox : public AActor
{
	GENERATED_BODY()

public:
	ACullingBoundBox();

protected:
	virtual void BeginPlay() override;
private:
	void SetActiveActor(AActor* Actor, bool bActive);
	
public:
	void FindOverlapActor();
	void SetActiveOn();
	void SetActiveOff();
	// 해당 위치가 Section에 위치해있는지
	bool CheckOverlapLocation(FVector Location);
	bool HasIgnoreTags	(AActor* Actor);

public:
	int32 GetSectionIndex() const
	{
		return MyIndexNumber;
	}
	bool GetActiveState() const
	{
		return bSectionActive;
	}
	TArray<int32> GetNeighborIndex() const
	{
		return NeighborIndex;
	}
	
protected:
	bool bSectionActive = false;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Bound_Box|Section")
	int32 MyIndexNumber{-1};


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bound_Box|Section")
	TObjectPtr<UBoxComponent> SectionBox;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SectionActors;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Bound_Box|Section")
	TArray<FName> IgnoreTags = {
		TEXT("Player"),
		TEXT("AlwaysLoaded"),
		TEXT("NoSectionCull")
	};
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Bound_Box|Section")
	TArray<int32> NeighborIndex;
};
