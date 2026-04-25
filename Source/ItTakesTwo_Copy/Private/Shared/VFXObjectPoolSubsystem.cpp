// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/VFXObjectPoolSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Shared/Actor/VFXProjectileObject.h"

#define MAX_VFXPOOLSIZE 40

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

}



TObjectPtr<AVFXProjectileObject> UVFXObjectPoolSubsystem::UseVFX_Projectile(FVFXSpawn_Info VFXInfo)
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

void UVFXObjectPoolSubsystem::UseVFX_Explosion(FVFXSpawn_Info VFXInfo)
{
	
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
