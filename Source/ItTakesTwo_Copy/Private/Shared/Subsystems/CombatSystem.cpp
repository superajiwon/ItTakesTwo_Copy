
#include "Shared/Subsystems/CombatSystem.h"

#include "Interfaces/Damagable.h"
#include "Shared/Struct/HitRequest.h"

bool UCombatSystem::ProcessHit(const FHitRequest& Request)
{
	if (!IsValidHitRequest(Request)) return false;
	
	// 한 공격에 여러번 타격 들어갈 수 도 있기 때문에 
	// 중복 히트 방지는 무적 상태로 무시
	
	if (IDamagable* DamagableTarget = Cast<IDamagable>(Request.Target))
	{
		DamagableTarget->Damage(Request.Damage, Request.Attacker);
		
		return true;
	}
	
	return false;
}

bool UCombatSystem::IsValidHitRequest(const FHitRequest& Request) const
{
	if (!IsValid(Request.Attacker) || !IsValid(Request.Target)) return false;
	if (Request.Attacker == Request.Target) return false;

	// 서버 권한 검증 — Listen Server 구조에서 판정은 서버에서만 처리
	if (!Request.Attacker->HasAuthority()) return false;

	return true;
}
