#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatSystem.generated.h"

struct FHitRequest;

UCLASS()
class ITTAKESTWO_COPY_API UCombatSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// 피격 판정 처리 (서버 전용)
	// - HasAuthority() 검증
	// - IDamagable 인터페이스로 데미지 전달
	// - 중복 히트 방지는 HealthComponent의 무적 상태(bIsInvincible)로 처리
	bool ProcessHit(const FHitRequest& Request);

private:
	// 피격 요청의 유효성 검사 (널 체크, 자기 자신, 서버 권한, 데미지 > 0)
	bool IsValidHitRequest(const FHitRequest& Request) const;
};
