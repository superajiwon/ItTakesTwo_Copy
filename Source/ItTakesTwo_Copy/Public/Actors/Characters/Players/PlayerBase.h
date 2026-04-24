
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CombatData")
	UPlayerActionData* ActionData;
	
	// 현재 폼에 맞는 공격 데이터를 반환하는 헬퍼	
	// 기본: NormalAttackData 반환
	// May: bIsUltimateForm 상태에 따라 Normal / Ultimate 반환
	virtual FAttackModeData* GetCurrentAttackData();
	
	// 궁극기 활성화 콜백 (virtual — MayCharacter가 override)
	// Server_ExecuteSkill에서 Ultimate(ActionType==3) 수신 시 서버에서 호출됩니다.
	virtual void OnUltimateActivated() {}
	
	// 현재 콤보 단계 (로컬 상태 — 소유 클라이언트/방장에서만 증가)
	int32 CurComboIndex = 0;
	
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
	
	virtual void Move(const FInputActionValue& Value);
	virtual void BaseAttack(const FInputActionValue& Value);
	virtual void SpecialAttack(const FInputActionValue& Value);
	virtual void Dash(const FInputActionValue& Value);
	virtual void Ultimate(const FInputActionValue& Value);
	
	virtual void TakeDamageAction();
};
