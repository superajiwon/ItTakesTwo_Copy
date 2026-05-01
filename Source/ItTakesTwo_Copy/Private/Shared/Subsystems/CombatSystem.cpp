
#include "Shared/Subsystems/CombatSystem.h"

#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/UltimateComponent.h"
#include "Interfaces/Damagable.h"
#include "Shared/Struct/HitRequest.h"

bool UCombatSystem::ProcessHit(const FHitRequest& Request)
{
	if (!IsValidHitRequest(Request)) return false;
	
	if (IDamagable* DamagableTarget = Cast<IDamagable>(Request.Target))
	{
		DamagableTarget->Damage(Request.Damage, Request.Attacker);
		
		// 플레이어 궁 게이지 
		if (APlayerBase* Player = Cast<APlayerBase>(Request.Attacker))
		{
			Player->GetUltimateComponent()->AddGauge(10);
		}
		
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
