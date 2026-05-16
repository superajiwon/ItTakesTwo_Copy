
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damagable.h"
#include "Interfaces/Healable.h"
#include "CharacterBase.generated.h"

class UHPComponent;
class UStatComponent;
class UWidgetComponent;
class AFloatingUIActor;

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
	
	virtual void PrintNetLog();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UHPComponent* HPComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UStatComponent* StatComp;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* HPUIComp;
	void InitHPBar();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlayerArrowComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Setting", meta=(AllowPrivateAccess = "true"))
	FName TargetName = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FLinearColor HPBarColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FLinearColor HPBarHitColor = FLinearColor::White;
	
public:
	UFUNCTION(BlueprintCallable, Category="Components")
	UHPComponent* GetHPComponent() { return HPComp; }
	UFUNCTION(BlueprintCallable, Category="Components")
	UStatComponent* GetStatComponent() { return StatComp; }
	UFUNCTION(BlueprintCallable, Category="Components")
	UWidgetComponent* GetHPUIComponent() const { return HPUIComp; }

	virtual void Heal(float HealAmount) override;
	virtual void Damage(float DamageAmount, AActor* Causer) override;
	
	UFUNCTION(BlueprintCallable, Category="Components")
	FName GetTargetName() { return TargetName; }
	UFUNCTION(BlueprintCallable, Category="Components")
	void SetTargetName(FName NameTag) { TargetName = NameTag; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	TSubclassOf<UCameraShakeBase> CamShake;
	
	void PlayCamShake(float Scale = 1.0f);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayCamShake(float Scale = 1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<AFloatingUIActor> FloatingUIClass;
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ShowDamageUI(float DamageAmount, FVector SpawnLocation, FLinearColor SpawnColor = FLinearColor::Red);
};
