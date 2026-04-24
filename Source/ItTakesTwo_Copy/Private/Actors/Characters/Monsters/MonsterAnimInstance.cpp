// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/MonsterAnimInstance.h"
#include "Actors/Characters/Monsters/MonsterBase.h"
#include "Components/HPComponent.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	CachedMonster = Cast<AMonsterBase>(TryGetPawnOwner());
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedMonster)
	{
		CachedMonster = Cast<AMonsterBase>(TryGetPawnOwner());
	}
	if (!CachedMonster)
	{
		Speed = 0.0f;
		bIsMoving = false;
		CurrentState = EMonsterState::Idle;
		return;
	}
	
	const FVector Velocity = CachedMonster->GetVelocity();
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	bIsMoving = Speed > 3.0f;
	CurrentState = CachedMonster->GetMonsterState();
	bIsDead = CachedMonster->GetHPComponent()->GetIsDead();
}

void UMonsterAnimInstance::AnimNotify_MontageEnd()
{
	if (!CachedMonster)
		return;
	
	CachedMonster->AnimNotify_MontageEnd();
}
