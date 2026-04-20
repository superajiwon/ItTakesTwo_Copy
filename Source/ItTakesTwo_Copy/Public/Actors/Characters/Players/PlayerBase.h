
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "PlayerBase.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ITTAKESTWO_COPY_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerBase();
	
public:
	UFUNCTION(BlueprintCallable, Category="Components")
	USkillComponent* GetSkillComponent() { return SkillComp; }
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USkillComponent* SkillComp;
	
public:
	// === Input ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	
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
