
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ITTAKESTWO_COPY_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerBase();
	
	UFUNCTION(BlueprintCallable, Category="Components")
	USkillComponent* GetSkillComponent() { return SkillComp; }
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USkillComponent* SkillComp;
	
	// UPROPERTY()
	// class ACameraManagerActor* CameraManager;
	
public:
	// === Input ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* IMC_PlayerMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_BaseAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_SpecialAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Ultimate;
	
	void Move(const FInputActionValue& Value);
	void BaseAttack(const FInputActionValue& Value);
	void SpecialAttack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Ultimate(const FInputActionValue& Value);
};
