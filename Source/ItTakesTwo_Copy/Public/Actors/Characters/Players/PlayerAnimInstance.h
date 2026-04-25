
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
		
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	bool bIsMoving;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	bool bIsMontagePlaying = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	bool bCanCombo = false;
	
	
	// === Anim Notify ===
	UFUNCTION()
	virtual void AnimNotify_DashOn() {};
	
	UFUNCTION()
	virtual void AnimNotify_SpecialOn() {};
	
	UFUNCTION()
	virtual void AnimNotify_SpecialOff() {};
	
	UFUNCTION()
	virtual void AnimNotify_UltimateOn() {};
};
