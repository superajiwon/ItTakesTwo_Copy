

#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_HandCollider.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


AToyOgre_HandCollider::AToyOgre_HandCollider()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicateMovement(false);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);

	SphereCollision->SetSphereRadius(80.f);
	SphereCollision->SetCollisionProfileName(TEXT("MonsterBody"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetGenerateOverlapEvents(false);

	Tags.Add(TEXT("Monster"));
}

void AToyOgre_HandCollider::Damage(float DamageAmount, AActor* Causer)
{
	if (!HasAuthority() || bBroken)
		return;

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);

	if (CurrentHP <= 0.f)
	{
		bBroken = true;
		DeactivateHand();

		if (OwnerOgre)
		{
			OwnerOgre->OnHandBroken(IsLeftHand);
		}
	}
}

void AToyOgre_HandCollider::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AToyOgre_HandCollider, CurrentHP);
	DOREPLIFETIME(AToyOgre_HandCollider, bBroken);
}
void AToyOgre_HandCollider::InitHand(AToyOgre_Monster* InOwnerOgre, bool IsLeftSide)
{
	OwnerOgre = InOwnerOgre;
	IsLeftHand = IsLeftSide;

	CurrentHP = MaxHP;
	bBroken = false;

	DeactivateHand();
}

void AToyOgre_HandCollider::ActivateHand()
{
	if (bBroken || !SphereCollision)
		return;

	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->UpdateOverlaps();
}

void AToyOgre_HandCollider::DeactivateHand()
{
	if (!SphereCollision)
		return;

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetGenerateOverlapEvents(false);
}

void AToyOgre_HandCollider::RegenHand()
{
	CurrentHP = MaxHP;
	bBroken = false;

	DeactivateHand();
}


void AToyOgre_HandCollider::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToyOgre_HandCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//
	// if (!SphereCollision)
	// 	return;
	//
	// const FVector Location = SphereCollision->GetComponentLocation();
	// const float Radius = SphereCollision->GetScaledSphereRadius();
	//
	// // 콜라이더 범위 출력
	// DrawDebugSphere(
	// 	GetWorld(),
	// 	Location,
	// 	Radius,
	// 	24,
	// 	bBroken ? FColor::Red : FColor::Green,
	// 	false,
	// 	0.f,
	// 	0,
	// 	2.f
	// );

	// HP 텍스트 출력
	// const FString DebugText = FString::Printf(
	// 	TEXT("%s Hand\nHP: %.0f / %.0f\nBroken: %s"),
	// 	IsLeftHand ? TEXT("Left") : TEXT("Right"),
	// 	CurrentHP,
	// 	MaxHP,
	// 	bBroken ? TEXT("true") : TEXT("false")
	// );
	//
	// DrawDebugString(
	// 	GetWorld(),
	// 	Location + FVector(0.f, 0.f, Radius + 50.f),
	// 	DebugText,
	// 	nullptr,
	// 	FColor::White,
	// 	0.f,
	// 	true
	// );
}

