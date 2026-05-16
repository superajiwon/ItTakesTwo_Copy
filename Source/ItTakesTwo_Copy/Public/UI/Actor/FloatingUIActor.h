
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingUIActor.generated.h"

class UFloatingUI;
class UWidgetComponent;

UCLASS()
class ITTAKESTWO_COPY_API AFloatingUIActor : public AActor
{
	GENERATED_BODY()

public:
	AFloatingUIActor();

protected:
	virtual void BeginPlay() override;
	
public:
	void ShowFloatingUI(FText Text, FLinearColor Color);

	void ActivateFloatingUI(FText Text, FLinearColor Color);
	void DeactivateFloatingUI();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* WidgetComp;

private:
	FTimerHandle LifeTimerHandle;

	UPROPERTY()
	UFloatingUI* CachedFloatingUI;
};
