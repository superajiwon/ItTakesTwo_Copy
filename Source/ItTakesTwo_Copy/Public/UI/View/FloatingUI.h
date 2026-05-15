
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingUI.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class ITTAKESTWO_COPY_API UFloatingUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void PlayFloating(FText Text, FLinearColor OutLinearColor);
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Floating;
	
	UPROPERTY(meta=(BindWidget))	
	UTextBlock* ShowText;
};

