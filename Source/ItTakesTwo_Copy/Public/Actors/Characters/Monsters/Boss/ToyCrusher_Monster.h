// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBase.h"
#include "ToyCrusher_Monster.generated.h"

class AMapObject_Button;
class UBoxComponent;
class UNiagaraSystem;

UCLASS()
class ITTAKESTWO_COPY_API AToyCrusher_Monster : public ABossBase
{
	GENERATED_BODY()

public:
	AToyCrusher_Monster();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	virtual void AnimNotify_CrusherAttack();
	virtual void Tick(float DeltaTime) override;
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category="VFXImpact")
	TObjectPtr<UNiagaraSystem> ImpactNiagara;

	UPROPERTY(EditDefaultsOnly, Category="VFXImpact")
	FVector ImpactExtents{200.f};

private:
	UPROPERTY(EditDefaultsOnly, Category="VFXImpact")
	float ImpactDamage{90000.f};
	
	UPROPERTY()
	bool bActive{false};
	
	UPROPERTY()
	bool bTravelRequested{false};
	
	
private:
	UPROPERTY()
	UBoxComponent* BoxComponent;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	TObjectPtr<AMapObject_Button> First_Button;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapObject_Button")
	TObjectPtr<AMapObject_Button> Second_Button;
	
};
