// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Attackable.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"
#include "UI/Actor/FloatingUIActor.h"
#include "UI/UIManager/FloatingUIPoolSubsystem.h"

AMapObject_Attackable::AMapObject_Attackable()
{
	PrimaryActorTick.bCanEverTick = true;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeometryCollection->SetCollisionProfileName(FName("MapObject"));
	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);

	Tags.Add(FName("Monster"));
}

void AMapObject_Attackable::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CurrentHP = MaxHP;
	}
	if (HitCollision)
		HitCollision->OnComponentBeginOverlap.AddDynamic(this,&AMapObject_Attackable::OnBeginOverlap);
	
}

void AMapObject_Attackable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// const FString ConStr = (GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetNetMode()==ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"));
	// const FString LogStr = FString::Printf(TEXT("%s\nHP : %.0f"), *ConStr, CurrentHP );
	// DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 100.0f, LogStr, nullptr, FColor::White, 0, true, 1);
	
	if (!HasAuthority() || bDestroyed)
		return;

	if (CurrentHP <= 0.f)
	{
		DestroyObject();
	}
}

void AMapObject_Attackable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMapObject_Attackable, CurrentHP);
	DOREPLIFETIME(AMapObject_Attackable, bDestroyed);
	DOREPLIFETIME(AMapObject_Attackable, LastHitLocation);
	DOREPLIFETIME(AMapObject_Attackable, LastHitDirection);
}

void AMapObject_Attackable::Damage(float DamageAmount, AActor* Causer)
{
	IDamagable::Damage(DamageAmount, Causer);
	if (!HasAuthority() || bDestroyed)
		return;

	if (Causer)
	{
		LastHitLocation = Causer->GetActorLocation();
		LastHitDirection = (GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal();
	}
	else
	{
		LastHitLocation = GetActorLocation();
		LastHitDirection = GetActorForwardVector();
	}
	ApplyDamage(DamageAmount);

	// Floating UI (데미지 UI)
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 88.0f);
	Multicast_ShowDamageUI(DamageAmount, SpawnLocation);
}

void AMapObject_Attackable::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
}

void AMapObject_Attackable::ApplyDamage(float Damage)
{
	if (!HasAuthority() || bDestroyed )
		return;
	
	CurrentHP -= Damage; 
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
	
	if (CurrentHP <= 0.f)
	{
		DestroyObject();
	}
}

void AMapObject_Attackable::DestroyObject()
{
	if (!HasAuthority() || bDestroyed)
		return;
	bDestroyed = true;
	CurrentHP = 0.f;

	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GeometryCollection)
	{
		GeometryCollection->SetCollisionProfileName(FName("MapObject_Crush"));
	}

	Multicast_PlayDestroySound(DestroySoundId);
	PlayDestroyEffect(LastHitLocation, LastHitDirection);

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AMapObject_Attackable::DestroyAfterDelay,
		5.f,
		false
	);
}


void AMapObject_Attackable::Multicast_ShowDamageUI_Implementation(float DamageAmount, FVector SpawnLocation)
{
	if (UWorld* World = GetWorld())
	{
		if (UFloatingUIPoolSubsystem* PoolSubsystem = World->GetSubsystem<UFloatingUIPoolSubsystem>())
		{
			if (AFloatingUIActor* FloatingActor = PoolSubsystem->GetFloatingUIActor(FloatingUIClass, SpawnLocation))
			{
				FloatingActor->ActivateFloatingUI(FText::AsNumber(DamageAmount), FLinearColor::Red);
			}
		}
	}
}


void AMapObject_Attackable::Multicast_PlayDestroySound_Implementation(FName SoundId)
{
	if (SoundId.IsNone())
		return;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USoundManagerSubsystem* SoundManager =
			GameInstance->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlaySFX2D(SoundId);
		}
	}
}


void AMapObject_Attackable::OnRep_CurrentHP()
{
}

void AMapObject_Attackable::OnRep_RegenTimer()
{
}

void AMapObject_Attackable::OnRep_Destroyed()
{
	if (bDestroyed)
	{
		if (HitCollision)
		{
			HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (GeometryCollection)
		{
			GeometryCollection->SetCollisionProfileName(FName("MapObject_Crush"));
		}
		PlayDestroyEffect(LastHitLocation, LastHitDirection);
	}
}

void AMapObject_Attackable::DestroyAfterDelay()
{
	if (!HasAuthority())
		return;

	Destroy();
}

void AMapObject_Attackable::PlayDestroyEffect(const FVector& HitLocation, const FVector& HitDirection)
{
	
	if (!GeometryCollection)
		return;

	GeometryCollection->SetCollisionProfileName(FName("MapObject_Crush"));
	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GeometryCollection->SetSimulatePhysics(true);

	const FVector Center = GeometryCollection->GetComponentLocation();
	const FVector Dir = HitDirection.GetSafeNormal();

	GeometryCollection->AddImpulse(
	Dir * 500.f, // 어디서 어느 방향으로 때렸는지 받아와야 할듯
	NAME_None,
	true
);

	// 살짝 퍼지는 힘
	GeometryCollection->AddRadialImpulse(
		HitLocation,
		500.f,
		300.f,
		ERadialImpulseFalloff::RIF_Linear,
		true
	);
}
