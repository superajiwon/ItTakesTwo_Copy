// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RoseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESTWO_COPY_API URoseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSelected = false;
	
	UFUNCTION(BlueprintCallable)
	void Select();
};
 