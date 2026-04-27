
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damagable.h"
#include "Interfaces/Healable.h"
#include "CharacterBase.generated.h"

class UHPComponent;
class UStatComponent;

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
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UHPComponent* HPComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStatComponent* StatComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Setting", meta=(AllowPrivateAccess = "true"))
	FName TargetName = "";

public:
	UFUNCTION(BlueprintCallable, Category="Components")
	UHPComponent* GetHPComponent() { return HPComp; }
	UFUNCTION(BlueprintCallable, Category="Components")
	UStatComponent* GetStatComponent() { return StatComp; }

	virtual void Heal(float HealAmount) override;
	virtual void Damage(float DamageAmount, AActor* Causer) override;
	
	UFUNCTION(BlueprintCallable, Category="Components")
	FName GetTargetName() { return TargetName; }
	UFUNCTION(BlueprintCallable, Category="Components")
	void SetTargetName(FName NameTag) { TargetName = NameTag; }
	
};
