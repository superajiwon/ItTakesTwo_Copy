// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/VFXObject.h"

#include "NiagaraComponent.h"
#include "Actors/Characters/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraSystem.h"

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
	if (VFXComponent)
	{
		VFXComponent->OnSystemFinished.RemoveDynamic(this, &AVFXObject::OnVFXSystemFinished);
		VFXComponent->OnSystemFinished.AddDynamic(this, &AVFXObject::OnVFXSystemFinished);
	}

	if (HasAuthority())
	{
		FinishVFXObject();
	}
	else if (!VFXRepState.bUsing)
	{
		FinishVisualState();
	}
}

void AVFXObject::OnVFXSystemFinished(UNiagaraComponent* FinishedComponent)
{
	if (!bUsing)
	{
		return;
	}

	if (FinishedComponent != VFXComponent)
	{
		return;
	}

	if (VFXInfo.VFXType == EVFXSpawnType::Explosion_Once)
	{
		FinishVFXObject();
	}
}

void AVFXObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 충돌 박스 그리기
	 switch (VFXInfo.CollisionInfo.CollisionShape)
	 {
	 case EVFXCollisionShape::Sphere:
	 	DrawDebugSphere(GetWorld(),CollisionSphereComponent->GetComponentLocation(),CollisionSphereComponent->GetScaledSphereRadius(),24,FColor::Blue,false,-1.f,0,2.f);
	 	break;
	
	 case EVFXCollisionShape::Box:
	 	DrawDebugBox(
	 		GetWorld(),CollisionBoxComponent->GetComponentLocation(),CollisionBoxComponent->GetScaledBoxExtent(),FColor::Blue,false,-1.f,0,2.f);
	 	break;
	
	 default:
	 	break;
	 }
	
	
	switch (VFXInfo.VFXType) 
	{
		case EVFXSpawnType::Projectile_Distance:
		{
			CheckDistance(DeltaTime);
			break;
		}
		case EVFXSpawnType::Projectile_LifeTime:
		case EVFXSpawnType::Explosion_LifeTime:
		{
			CheckLifeTime(DeltaTime);
			break;
		}
		case EVFXSpawnType::Explosion_Once:
			break;
		case EVFXSpawnType::Projectile_Homing:
			break;
		case EVFXSpawnType::Projectile_Hit:
		{
			CheckDistance(DeltaTime);
			break;
		}	
		case EVFXSpawnType::End:
			break;
	}


}

void AVFXObject::UseVFXObject(const FVFXSpawn_Info& SpawnInfo)
{
	VFXInfo = SpawnInfo;

	if (HasAuthority())
	{
		SetVFXRepStateFromSpawnInfo(SpawnInfo);
	}

	ApplyVisualState(SpawnInfo);

	if (HasAuthority() && VFXInfo.CollisionInfo.bAttack)
	{
		UseCollision();
	}

	ForceNetUpdate();
}

void AVFXObject::FinishVFXObject()
{
	if (HasAuthority() && VFXInfo.CollisionInfo.bAttack)
	{
		FinishCollision();
	}

	if (HasAuthority())
	{
		VFXRepState.bUsing = false;
		++VFXRepState.ActivationId;
		ForceNetUpdate();
	}

	FinishVisualState();

	VFXInfo = FVFXSpawn_Info();
}

void AVFXObject::CheckLifeTime(float DeltaTime)
{
	CurrentLifeTime += DeltaTime;
	if (VFXInfo.LifeTime < CurrentLifeTime)
	{
		// 사용 종료
		FinishVFXObject();
		CurrentLifeTime = 0.f;
	}
}

void AVFXObject::CheckDistance(float DeltaTime)
{
	if (VFXInfo.LifeDistance <= 0.f)
		return;
	// 시작 위치에서 현재 위치의 거리 계산
	float Dist = FVector::Dist2D(VFXInfo.StartLocation, GetActorLocation());
	if (VFXInfo.LifeDistance <= Dist)
	{
		FinishVFXObject();
	}
}
void AVFXObject::UseCollision()
{
	const FVFXCollision_Info& Col = VFXInfo.CollisionInfo;

	switch (Col.CollisionShape)
	{
	case EVFXCollisionShape::Sphere:
		{
			if (CollisionSphereComponent)
			{
				CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				CollisionSphereComponent->SetSphereRadius(Col.SphereRadius);
				CollisionSphereComponent->SetCollisionProfileName(Col.CollisionPresetName);
				CollisionSphereComponent->SetGenerateOverlapEvents(true);
				CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				CollisionSphereComponent->UpdateOverlaps();
			}
			break;
		}

	case EVFXCollisionShape::Box:
		{
			if (CollisionBoxComponent)
			{
				CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				CollisionBoxComponent->SetBoxExtent(Col.BoxExtents);
				CollisionBoxComponent->SetCollisionProfileName(Col.CollisionPresetName);
				CollisionBoxComponent->SetGenerateOverlapEvents(true);
				CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				CollisionBoxComponent->UpdateOverlaps();
			}
			break;
		}

	case EVFXCollisionShape::None:
	default:
		break;
	}
}
void AVFXObject::FinishCollision()
{
	switch (VFXInfo.CollisionInfo.CollisionShape)
	{
	case EVFXCollisionShape::Sphere:
		if (CollisionSphereComponent)
		{
			CollisionSphereComponent->SetGenerateOverlapEvents(false);
			CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	case EVFXCollisionShape::Box:
		if (CollisionBoxComponent)
		{
			CollisionBoxComponent->SetGenerateOverlapEvents(false);
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	default:
		break;
	}
}

void AVFXObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVFXObject, VFXRepState);
	
}

void AVFXObject::OnRep_VFXRepState()
{
	UE_LOG(LogTemp, Warning, TEXT("[VFX OnRep] %s bUsing=%d Asset=%s ActivationId=%d"),
		*GetName(),
		VFXRepState.bUsing,
		*GetNameSafe(Cast<UObject>(VFXRepState.NiagaraAsset)),
		VFXRepState.ActivationId);
	
	if (!VFXRepState.bUsing)
	{
		FinishVisualState();
		return;
	}

	FVFXSpawn_Info SpawnInfo;
	SpawnInfo.VFXType = VFXRepState.VFXType;
	SpawnInfo.NiagaraAsset = VFXRepState.NiagaraAsset;
	SpawnInfo.StartLocation = VFXRepState.StartLocation;
	SpawnInfo.StartRotation = VFXRepState.StartRotation;
	SpawnInfo.Direction = VFXRepState.Direction;
	SpawnInfo.Speed = VFXRepState.Speed;
	SpawnInfo.LifeTime = VFXRepState.LifeTime;
	SpawnInfo.LifeDistance = VFXRepState.LifeDistance;
	SpawnInfo.TargetActor = VFXRepState.TargetActor;

	ApplyVisualState(SpawnInfo);
}

void AVFXObject::SetVFXRepStateFromSpawnInfo(const FVFXSpawn_Info& SpawnInfo)
{
	if (!HasAuthority())
	{
		return;
	}

	VFXRepState.bUsing = true;
	VFXRepState.NiagaraAsset = SpawnInfo.NiagaraAsset;
	VFXRepState.VFXType = SpawnInfo.VFXType;
	VFXRepState.StartLocation = SpawnInfo.StartLocation;
	VFXRepState.StartRotation = SpawnInfo.StartRotation;
	VFXRepState.Direction = SpawnInfo.Direction;
	VFXRepState.Speed = SpawnInfo.Speed;
	VFXRepState.LifeTime = SpawnInfo.LifeTime;
	VFXRepState.LifeDistance = SpawnInfo.LifeDistance;
	VFXRepState.TargetActor = SpawnInfo.TargetActor;
	++VFXRepState.ActivationId;
}

void AVFXObject::ApplyVisualState(const FVFXSpawn_Info& SpawnInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("[VFX ApplyVisual] %s Auth=%d Asset=%s Loc=%s"),
	*GetName(),
	HasAuthority(),
			*GetNameSafe(Cast<UObject>(VFXRepState.NiagaraAsset)),
	*SpawnInfo.StartLocation.ToString());
	VFXInfo = SpawnInfo;

	SetActorLocationAndRotation(SpawnInfo.StartLocation, SpawnInfo.StartRotation);

	if (VFXComponent)
	{
		VFXComponent->SetAsset(SpawnInfo.NiagaraAsset);
		VFXComponent->Activate(true);
	}

	Initialize_ForStart();

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void AVFXObject::FinishVisualState()
{
	UE_LOG(LogTemp, Warning, TEXT("[VFX FinishVisual] %s Auth=%d"),
			*GetName(),
			HasAuthority());
	if (VFXComponent)
	{
		VFXComponent->Deactivate();
	}

	bUsing = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

