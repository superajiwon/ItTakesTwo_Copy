
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodyUltimateBox.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API ACodyUltimateBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACodyUltimateBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
