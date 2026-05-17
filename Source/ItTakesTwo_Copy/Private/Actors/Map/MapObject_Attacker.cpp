// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Attacker.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Components/SceneComponent.h"


AMapObject_Attacker::AMapObject_Attacker()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(TEXT("HitBoxComponent"));
	HitBoxComponent->SetupAttachment(RootComponent);
	HitBoxComponent->SetDamage(Damage);
	HitBoxComponent->bAutoResetEndOverlap = true;

	// 히트박스는 충돌 판정용이라 게임 화면에 보이면 안 됨
	HitBoxComponent->SetVisibility(false, true);
	HitBoxComponent->SetHiddenInGame(true, true);
}

void AMapObject_Attacker::BeginPlay()
{
	Super::BeginPlay();

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* PrimitiveComp : PrimitiveComponents)
	{
		if (!PrimitiveComp)
			continue;

		if (PrimitiveComp->IsA<UMeshComponent>() || PrimitiveComp->IsA<UNiagaraComponent>())
			continue;

		PrimitiveComp->SetVisibility(false, true);
		PrimitiveComp->SetHiddenInGame(true, true);
	}

	if (SplineActor)
	{
		SplineComponent = SplineActor->FindComponentByClass<USplineComponent>();
	}

	if (HasAuthority() && HitBoxComponent)
	{
		FHitComp_Info HitInfo(
			FName(TEXT("MapObject")),
			FName(TEXT("MonsterWeapon")),
			FVector::ZeroVector,
			FVector(100.f, 100.f, 100.f)
		);

		HitBoxComponent->InitializeHitComp(HitInfo, FName(TEXT("Player")));
		HitBoxComponent->SetDamage(static_cast<int32>(Damage));
		HitBoxComponent->CollisionOn();

		HitBoxComponent->SetVisibility(false, true);
		HitBoxComponent->SetHiddenInGame(true, true);
	}
	
	if (!SplineComponent)
	{
		SetActorTickEnabled(false);
		return;
	}

	CurrentDistance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(
		SplineComponent->FindInputKeyClosestToWorldLocation(GetActorLocation())
	);
}

void AMapObject_Attacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
		return;

	if (!SplineComponent)
		return;
	
	const float SplineLength = SplineComponent->GetSplineLength();
	if (SplineLength <= 0.f)
		return;

	CurrentDistance += Speed * DeltaTime * MoveDirection;
	if (CurrentDistance >= SplineLength)
	{
		CurrentDistance = SplineLength;
		MoveDirection = -1;
	}
	else if (CurrentDistance <= 0.f)
	{
		CurrentDistance = 0.f;
		MoveDirection = 1;
	}
	const FVector NewLocation =
	SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	SetActorLocation(NewLocation);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(Rotation );
}

void AMapObject_Attacker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopLoopSound();
	Super::EndPlay(EndPlayReason);
}

void AMapObject_Attacker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	
	DOREPLIFETIME(AMapObject_Attacker, bAttackerActive);
}

void AMapObject_Attacker::StartLoopSound()
{
	if (!LoopSound)
		return;

	if (LoopAudioComponent && LoopAudioComponent->IsPlaying())
		return;

	LoopAudioComponent = UGameplayStatics::SpawnSoundAttached(
		LoopSound,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		EAttachLocation::KeepRelativeOffset,
		true
	);
}

void AMapObject_Attacker::StopLoopSound()
{
	if (!LoopAudioComponent)
		return;

	if (LoopAudioComponent->IsPlaying())
	{
		LoopAudioComponent->FadeOut(SoundFadeOutTime, 0.0f);
	}

	LoopAudioComponent = nullptr;
}
void AMapObject_Attacker::SetAttackerActive(bool bActive)
{
	if (HasAuthority())
	{
		bAttackerActive = bActive;
		ApplyAttackerActive(bActive, true);
	}
	else
	{
		// 클라는 시각/사운드만 로컬 컬링
		ApplyAttackerActive(bActive, false);
	}
}

void AMapObject_Attacker::OnRep_AttackerActive()
{
	ApplyAttackerActive(bAttackerActive, false);
}


void AMapObject_Attacker::ApplyAttackerActive(bool bActive, bool bApplyGameplay)
{
	SetActorHiddenInGame(!bActive);

	TArray<USceneComponent*> SceneComponents;
	GetComponents<USceneComponent>(SceneComponents);

	for (USceneComponent* Comp : SceneComponents)
	{
		if (!Comp)
			continue;

		if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Comp))
		{
			if (!PrimitiveComp->IsA<UMeshComponent>() && !PrimitiveComp->IsA<UNiagaraComponent>())
			{
				PrimitiveComp->SetVisibility(false, true);
				PrimitiveComp->SetHiddenInGame(true, true);
				continue;
			}
		}
		
		// HitBoxComponent는 충돌만 사용하고 화면에는 절대 보이지 않게 유지
		if (Comp == HitBoxComponent)
		{
			Comp->SetVisibility(false, true);
			Comp->SetHiddenInGame(true, true);
			continue;
		}

		Comp->SetVisibility(bActive, true);
		Comp->SetHiddenInGame(!bActive, true);

		if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Comp))
		{
			if (bActive)
			{
				NiagaraComp->Activate(true);
			}
			else
			{
				NiagaraComp->DeactivateImmediate();
			}
		}
	}

	if (bActive)
	{
		StartLoopSound();
	}
	else
	{
		StopLoopSound();
	}

	if (!bApplyGameplay)
		return;

	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);

	if (HitBoxComponent)
	{
		if (bActive)
		{
			HitBoxComponent->CollisionOn();
		}
		else
		{
			HitBoxComponent->CollisionOff();
		}

		// CollisionOn 이후에도 화면에는 보이지 않게 다시 고정
		HitBoxComponent->SetVisibility(false, true);
		HitBoxComponent->SetHiddenInGame(true, true);
	}
	
	
}
