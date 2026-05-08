
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "CodyUltimateBox.generated.h"

class UDotHitBoxComponent;
class UNiagaraComponent;

UCLASS()
class ITTAKESTWO_COPY_API ACodyUltimateBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACodyUltimateBox();

	
protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="InGame")
	void SetDistance(float Dist) { Distance = Dist; }
	
	UFUNCTION(BlueprintCallable, Category="InGame")
	void SetDamage(int32 Damage);
	UFUNCTION(BlueprintCallable, Category="InGame")
	void CollisionOn();
	UFUNCTION(BlueprintCallable, Category="InGame")
	void CollisionOff();
	
	UFUNCTION()
	void OnRep_IsBeamActive();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> CenterComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDotHitBoxComponent> AttackBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara")
	TObjectPtr<UNiagaraComponent> Beam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Niagara")
	TObjectPtr<UNiagaraComponent> Impact;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGame")
	FName BeamLengthParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGame")
	float Distance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGame")
	float TargetDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGame")
	float MaxDistance = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGame")
	float LerpSpeed = 10.0f;

	UPROPERTY(ReplicatedUsing = OnRep_IsBeamActive)
	bool bIsBeamActive;
};
