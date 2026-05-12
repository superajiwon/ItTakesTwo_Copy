// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CullingSection_EntranceBox.generated.h"

class UBoxComponent;
class ACullingBoundBoxManager;
class ARespawn_TargetPoint;

UCLASS()
class ITTAKESTWO_COPY_API ACullingSection_EntranceBox : public AActor
{
	GENERATED_BODY()

public:
	ACullingSection_EntranceBox();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Culling|TriggerBox")
	int32 SectionIndex{-1};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Culling|TriggerBox")
	FName PlayerTag = TEXT("Player");
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Culling|TriggerBox")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Culling|TriggerBox")
	TObjectPtr<ACullingBoundBoxManager> CullingManager;
	
	UPROPERTY(BlueprintReadWrite, Category = "Culling|RespawnPoint")
	TObjectPtr<ARespawn_TargetPoint> CurRespawnPointMay;
	UPROPERTY(BlueprintReadWrite, Category = "Culling|RespawnPoint")
	TObjectPtr<ARespawn_TargetPoint> CurRespawnPointCody;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Culling|Camera")
	bool bChangeCameraRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Culling|Camera", meta=(EditCondition="bChangeCameraRotation"))
	FRotator TargetCameraRotation = FRotator(-45.f, -45.f, 0.f);
};
