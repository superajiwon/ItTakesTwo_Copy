#pragma once
#include "CoreMinimal.h"
#include "HitComp_Info.generated.h"

USTRUCT(BlueprintType)
struct FHitComp_Info 
{
	GENERATED_BODY()
	
public: // 구조체 생성자로 초기화
	FHitComp_Info()
	: HitTagName(NAME_None), CollisionProfileName(NAME_None), HitBoxLocation(FVector::ZeroVector), HitBoxExtents(FVector::ZeroVector)
	{	}
	
	FHitComp_Info(FName TagName, FName ProfileName, FVector Location, FVector Extents)
		: HitTagName(TagName), CollisionProfileName(ProfileName), HitBoxLocation(Location), HitBoxExtents(Extents)
	{	}
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitTagName{NAME_None};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionProfileName{NAME_None};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxLocation{FVector::ZeroVector};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitBoxExtents{FVector::ZeroVector};
};
