
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITTAKESTWO_COPY_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatComponent();

protected:
	virtual void BeginPlay() override;

	// 네트워크 변수 복제 규칙 정의 함수
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
public:
	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetMoveSpeed() const { return MoveSpeed; }
	
	UFUNCTION(BlueprintCallable, Category="Stat")
	float GetAttackPower() const { return AttackPower; }
	
//! 변수
private:   
	UPROPERTY(EditAnywhere, Replicated, Category="Stat")
	float MoveSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, Replicated, Category="Stat")
	float AttackPower = 10.0f;
	
};
