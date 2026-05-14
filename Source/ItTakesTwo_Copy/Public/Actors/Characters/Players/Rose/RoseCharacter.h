
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSelect = false;
};
