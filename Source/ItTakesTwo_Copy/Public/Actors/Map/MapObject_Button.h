// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_Button.generated.h"


class UBoxComponent;

UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Button : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Button();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	virtual void Tick(float DeltaTime) override;
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayButtonPressSound();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY()
	int32 PlayerCount{0};
};
