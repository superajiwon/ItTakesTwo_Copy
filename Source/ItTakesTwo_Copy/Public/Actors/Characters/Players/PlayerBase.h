
#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/Players/PlayerActionData.h"
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
	
public:
	// === Action ===	
	// 액션 데이터 (DataAsset)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Data")
	UPlayerActionData* ActionData;
	
	// 현재 폼에 맞는 공격 데이터를 반환하는 헬퍼	
	// 기본: NormalAttackData 반환
	// May: bIsUltimateForm 상태에 따라 Normal / Ultimate 반환
	virtual FAttackModeData* GetCurrentAttackData();
	
	// 궁극기 활성화 콜백 (MayCharacter)
	virtual void OnUltimateActivated() {}
	
	// Base Collider 제어
	UFUNCTION(BlueprintCallable, Category = "Combat|Data")
	virtual void SetWeaponCollision(bool bEnable) {}
	
	// === Combo ===
	// 현재 콤보 단계 (로컬 상태 — 소유 클라이언트/방장에서만 증가)
	int32 CurComboIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Combo")
	bool bIsAttacking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Combo")
	bool bCanCombo = false;
	
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void ResetCombo();
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_TakeDamageTemp;
	
	// === Input Action === 
	virtual void Move(const FInputActionValue& Value);
	virtual void BaseAttack(const FInputActionValue& Value);
	virtual void SpecialAttack(const FInputActionValue& Value);
	virtual void Dash(const FInputActionValue& Value);
	virtual void Ultimate(const FInputActionValue& Value);
	
	virtual void TakeDamageAction();
};
