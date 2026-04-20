
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damagable.h"
#include "Interfaces/Healable.h"
#include "CharacterBase.generated.h"

class UHPComponent;
class UStatComponent;
class USkillComponent;

UCLASS()
class ITTAKESTWO_COPY_API ACharacterBase : public ACharacter, public IDamagable, public IHealable
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Components")
	UHPComponent* GetHPComponent() { return HPComp; }
	UFUNCTION(BlueprintCallable, Category="Components")
	UStatComponent* GetStatComponent() { return StatComp; }

	virtual void OnHit();
	virtual void OnDie();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UHPComponent* HPComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStatComponent* StatComp;
};
