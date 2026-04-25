// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/VFXObject.h"

#include "NiagaraComponent.h"
#include "Actors/Characters/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AVFXObject::AVFXObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;

	VFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComponent"));
	if (VFXComponent)
	{
		VFXComponent->SetupAttachment(RootComponent);
		VFXComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		VFXComponent->SetCollisionProfileName(TEXT("NoCollision"));
		VFXComponent->SetGenerateOverlapEvents(false);
		VFXComponent->Deactivate();
		VFXComponent->SetAutoActivate(false);
	}
	
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereComponent"));
	if (CollisionSphereComponent)
	{
		CollisionSphereComponent->SetupAttachment(RootComponent);
		CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionSphereComponent->SetGenerateOverlapEvents(false);
	}
	
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
	if (CollisionBoxComponent)
	{
		CollisionBoxComponent->SetupAttachment(RootComponent);
		CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBoxComponent->SetGenerateOverlapEvents(false);
	}
	


}

void AVFXObject::BeginPlay()
{
	Super::BeginPlay();
	// SetActorHiddenInGame(true);
	// SetActorTickEnabled(false);

	FinishVFXObject();
}

void AVFXObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 충돌 박스 그리기
	if (!VFXInfo.bUseBoxCollision)
	{
		DrawDebugSphere(
		   GetWorld(),CollisionSphereComponent->GetComponentLocation(),  CollisionSphereComponent->GetScaledSphereRadius(),
		   24, FColor::Blue, false,-1.f,0,2.f );
	}
	else
	{
		DrawDebugBox(GetWorld(),CollisionBoxComponent->GetComponentLocation(),CollisionBoxComponent->GetScaledBoxExtent(),
			FColor::Blue,false,-1.f,0,2.f);
	}
	
	
	switch (VFXInfo.VFXType) 
	{
		case EAttackType::Projectile_Distance:
		{
			CheckDistance(DeltaTime);
			break;
		}
		case EAttackType::Projectile_LifeTime:
		case EAttackType::Explosion_LifeTime:
		{
			CheckLifeTime(DeltaTime);
			break;
		}
		case EAttackType::Explosion_Once:
			break;
		case EAttackType::Projectile_Hit:
		{
			CheckDistance(DeltaTime);
			break;
		}	
		case EAttackType::End:
			break;
	}


}

void AVFXObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	VFXInfo = SpawnInfo;
	VFXComponent->SetAsset(VFXInfo.NiagaraAsset);
	VFXComponent->Activate(true);
	// 시작을 위한 공통 변수 초기화
	Initialize_ForStart();
	// 공격용이면 콜리전 켜라
	if (VFXInfo.bAttack)
	{
		UseCollision();
	}
	
	
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorLocationAndRotation(VFXInfo.StartLocation, VFXInfo.StartRotation);
	ForceNetUpdate();
}

void AVFXObject::FinishVFXObject()
{
	// 콜리전 설정 끄기
	if (VFXInfo.bAttack)
	{
		FinishCollision();
	}
	VFXComponent->Deactivate();
	VFXInfo = FVFXSpawn_Info();
	bUsing = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
}

void AVFXObject::CheckLifeTime(float DeltaTime)
{
	CurrentLifeTime += DeltaTime;
	if (VFXInfo.Life_Time < CurrentLifeTime)
	{
		// 사용 종료
		FinishVFXObject();
		CurrentLifeTime = 0.f;
	}
}

void AVFXObject::CheckDistance(float DeltaTime)
{
	if (VFXInfo.Life_Distance <= 0.f)
		return;
	// 시작 위치에서 현재 위치의 거리 계산
	float Dist = FVector::Dist2D(VFXInfo.StartLocation, GetActorLocation());
	if (VFXInfo.Life_Distance <= Dist)
	{
		FinishVFXObject();
	}
}

void AVFXObject::UseCollision()
{
	if (VFXInfo.bUseBoxCollision)
	{
		if (CollisionBoxComponent)
		{
			CollisionBoxComponent->SetBoxExtent(VFXInfo.Extents);
			CollisionBoxComponent->SetCollisionProfileName(VFXInfo.CollisionPresetName);
			CollisionBoxComponent->SetGenerateOverlapEvents(true);
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CollisionBoxComponent->UpdateOverlaps();
		}
	}
	else
	{
		if (CollisionSphereComponent)
		{
			CollisionSphereComponent->SetSphereRadius(VFXInfo.SphereRadius);
			CollisionSphereComponent->SetCollisionProfileName(VFXInfo.CollisionPresetName);
			CollisionSphereComponent->SetGenerateOverlapEvents(true);
			CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CollisionSphereComponent->UpdateOverlaps();
		}
	}
}

void AVFXObject::FinishCollision()
{
	if (VFXInfo.bUseBoxCollision)
	{
		if (CollisionBoxComponent)
		{
			CollisionBoxComponent->SetGenerateOverlapEvents(false);
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		if (CollisionSphereComponent)
		{
			CollisionSphereComponent->SetGenerateOverlapEvents(false);
			CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

