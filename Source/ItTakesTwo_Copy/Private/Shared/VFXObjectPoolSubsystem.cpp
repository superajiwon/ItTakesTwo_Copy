// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/VFXObjectPoolSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Shared/Actor/VFXExplosionObject.h"
#include "Shared/Actor/VFXProjectileObject.h"

#define MAX_VFXPOOLSIZE 20

void UVFXObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!GetWorld())
	{
		return;
	}

	for (int32 i = 0; i < MAX_VFXPOOLSIZE; ++i)
	{
		AVFXProjectileObject* Projectile = GetWorld()->SpawnActor<AVFXProjectileObject>(AVFXProjectileObject::StaticClass());
		if (Projectile)
		{
			ProjectileList.Add(Projectile);
		}
	}
	for (int32 i = 0; i < MAX_VFXPOOLSIZE; ++i)
	{
		AVFXExplosionObject* Explosion = GetWorld()->SpawnActor<AVFXExplosionObject>(AVFXExplosionObject::StaticClass());
		if (Explosion)
		{
			ExplosionList.Add(Explosion);
		}
	}
}



TObjectPtr<AVFXProjectileObject> UVFXObjectPoolSubsystem::UseVFX_Projectile(const FVFXSpawn_Info& VFXInfo)
{
	for (AVFXProjectileObject* Projectile : ProjectileList)
	{
		if (Projectile && !Projectile->IsUsing())
		{
			Projectile->UseVFXObject(VFXInfo);
			return Projectile;
		}
	}
	return nullptr;
}

TObjectPtr<AVFXExplosionObject> UVFXObjectPoolSubsystem::UseVFX_Explosion(const FVFXSpawn_Info& VFXInfo)
{
	for (AVFXExplosionObject* Explosion : ExplosionList)
	{
		if (Explosion && !Explosion->IsUsing())
		{
			Explosion->UseVFXObject(VFXInfo);
			return Explosion;
		}
	}
	return nullptr;
}




void UVFXObjectPoolSubsystem::Deinitialize()
{
	for (AVFXProjectileObject* Projectile : ProjectileList)
	{
		if (IsValid(Projectile))
		{
			Projectile->Destroy();
		}
	}
	ProjectileList.Empty();
	
	for (AVFXExplosionObject* Explosion : ExplosionList)
	{
		if (IsValid(Explosion))
		{
			Explosion->Destroy();
		}
	}
	ExplosionList.Empty();

	Super::Deinitialize();
}

bool UVFXObjectPoolSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UVFXObjectPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// TArray<AActor*> FoundActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVFXProjectileObject::StaticClass(), FoundActors);
	// if (FoundActors.Num() > 0)
	// 	return false;
	return true;
}
