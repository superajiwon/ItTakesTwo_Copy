#pragma once

#include "CoreMinimal.h"
#include "HitRequest.generated.h"

USTRUCT()
struct FHitRequest
{
	GENERATED_BODY()
	
public:
	FHitRequest() {}
	FHitRequest(AActor* InAttacker, AActor* InTarget, int32 InDamage, const FVector InLocation)
		: Attacker(InAttacker), Target(InTarget), Damage(InDamage), HitLocation(InLocation)
	{}

public:
	UPROPERTY() // 공격자
	TObjectPtr<AActor> Attacker = nullptr;
	
	UPROPERTY() // 피격자
	TObjectPtr<AActor> Target = nullptr;
	
	int32 Damage = 0;
	
	// 데미지 숫자 띄우거나 이펙트 띄울 충돌 위치
	FVector HitLocation = FVector::ZeroVector;
};

