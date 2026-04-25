
#pragma once

#include "CoreMinimal.h"
#include "Struct/FVFXSpawn_Info.h"
#include "Subsystems/WorldSubsystem.h"
#include "VFXObjectPoolSubsystem.generated.h"

class AVFXProjectileObject;
UCLASS()
class ITTAKESTWO_COPY_API UVFXObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
public:
	TObjectPtr<AVFXProjectileObject> UseVFX_Projectile(FVFXSpawn_Info VFXInfo);
	void UseVFX_Explosion(FVFXSpawn_Info VFXInfo);
	
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AVFXProjectileObject>> ProjectileList;
	
	
};
