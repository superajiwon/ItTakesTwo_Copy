
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UltimateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltimateFinishSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITTAKESTWO_COPY_API UUltimateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUltimateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnUltimateFinishSignature OnUltimateFinish;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ultimate")
	float MaxUltimateGauge = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Ultimate")
	float CurUltimateGauge = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ultimate")
	float DecreasePerSec = 10.0f; // 소모 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_IsUltimateActive, Category="Ultimate")
	bool bIsUltimateActive = false;
	
	UFUNCTION()
	void OnRep_IsUltimateActive();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ultimate")
	bool bCanUltimate = false;
	
	void AddGauge(float GaugeAmount);
	
	bool CanUseUltimate();
	
	void ActivateUltimate();
	
	UFUNCTION(BlueprintCallable, Category="Ultimate")
	void EndUltimate();
};
