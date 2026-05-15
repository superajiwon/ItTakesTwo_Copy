
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoseCharacter.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API ARoseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARoseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSelect = false;
	
	// UFUNCTION(BlueprintCallable)
	// void Selected();
	
	// 서버가 호출하면 모든 클라이언트에서 실행됨
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Selected();
	
};
