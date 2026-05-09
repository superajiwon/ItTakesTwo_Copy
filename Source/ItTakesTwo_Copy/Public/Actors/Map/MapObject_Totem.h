#pragma once

#include "CoreMinimal.h"
#include "MapObjectBase.h"
#include "MapObject_Totem.generated.h"

class ALevelSequenceActor;
class UBoxComponent;
class ANiagaraActor;
UCLASS()
class ITTAKESTWO_COPY_API AMapObject_Totem : public AMapObjectBase
{
	GENERATED_BODY()

public:
	AMapObject_Totem();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UFUNCTION()
	void OnActivationBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnRep_Activated();

	void ActivateTotem();
	void PlayChainSequence();
	
	
public:
	bool IsActivated() const
	{
		return bActivated;
	}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Totem|MapObject")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Niagara|VFX")
	TObjectPtr<ANiagaraActor> PlacedNiagaraActor1;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Niagara|VFX")
	TObjectPtr<ANiagaraActor> PlacedNiagaraActor2;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Niagara|VFX")
	TObjectPtr<ANiagaraActor> PlacedNiagaraActor3;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Niagara|VFX")
	TObjectPtr<ANiagaraActor> PlacedNiagaraActor4;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Totem|Sequence")
	TObjectPtr<ALevelSequenceActor> ChainSequenceActor;


protected:
	UPROPERTY(ReplicatedUsing=OnRep_Activated, BlueprintReadOnly, Category="Totem|MapObject")
	bool bActivated = false;
	
};
