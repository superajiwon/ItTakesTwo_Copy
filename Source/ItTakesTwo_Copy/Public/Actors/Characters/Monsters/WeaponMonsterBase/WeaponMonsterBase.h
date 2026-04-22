// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Monsters/MonsterBase.h"
#include "WeaponMonsterBase.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AWeaponMonsterBase : public AMonsterBase
{
	GENERATED_BODY()

public:
	AWeaponMonsterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> RightHand_WeaponMeshComponent;
	
};
