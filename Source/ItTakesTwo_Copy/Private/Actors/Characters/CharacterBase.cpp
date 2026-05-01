
#include "Actors/Characters/CharacterBase.h"

#include "Components/HPComponent.h"
#include "Components/StatComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 액터 자체의 복제 활성화
	bReplicates = true;
	
	HPComp = CreateDefaultSubobject<UHPComponent>(TEXT("HPComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PrintNetLog();
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::Heal(float HealAmount)
{
	IHealable::Heal(HealAmount);
	
	GetHPComponent()->ApplyHeal(HealAmount);
}

void ACharacterBase::Damage(float DamageAmount, AActor* Causer)
{
	IDamagable::Damage(DamageAmount, Causer);
	
	GetHPComponent()->ApplyDamage(DamageAmount, Causer);
}

void ACharacterBase::PrintNetLog()
{
	const FString LogStr = FString::Printf(TEXT("HP : %.0f"), GetHPComponent()->GetCurHP());
	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}
