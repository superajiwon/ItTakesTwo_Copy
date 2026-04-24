
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "PlayerActionData.generated.h"


// ============================================================
//  콤보 한 단계에 재생할 수 있는 몽타주 묶음
//    - 배열이 1개면 항상 그 몽타주 재생
//    - 배열이 여러 개면 랜덤으로 하나 선택 재생 (Cody 전용)
// ============================================================
USTRUCT(BlueprintType)
struct FComboStepData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TArray<UAnimMontage*> Montages; // 랜덤 재생할 몽타주
};

// ============================================================
//  하나의 공격 모드 전체 데이터 (Normal / Ultimate 공용)
//    - BasicAttackCombos 배열 크기 = 최대 콤보 수
//      ex) Cody Normal : 3칸, 각 칸 몽타주 3개
//          May  Normal : 3칸, 각 칸 몽타주 1개
//          May  Ult    : 1칸, 몽타주 1개 (궁극기 중 기본공격)
// ============================================================
USTRUCT(BlueprintType)
struct FAttackModeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TArray<FComboStepData> BasicAttackCombos;	// 기본 공격
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* SpecialAttackMontage;			// 스킬 공격
};


// ============================================================
//  캐릭터 1명분의 모든 액션 데이터를 담는 DataAsset
//    에디터에서 DA_Cody, DA_May 2개를 만들어 각 BP에 할당
// ============================================================
// PrimaryDataAsset 쓰는 이유 : 
// 비동기 로드에 최적화 되어있어서(Async Load) 동적으로 액션 데이터를 갈아끼워야 하는 지금과 같은 경우에 사용하는 것이 좋다.
UCLASS()	 
class ITTAKESTWO_COPY_API UPlayerActionData : public UPrimaryDataAsset 
{
	GENERATED_BODY()
	
public:
	// 평상시 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	FAttackModeData NormalAttackData;
	
	// 궁극기 사용 시 공격 -> May 전용 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	FAttackModeData UltimateAttackData;
	
	// 대쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	UAnimMontage* DashMontage;
	
	// 궁극기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	UAnimMontage* UltimateMontage;
	
	// 피격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	FComboStepData TakeDamageData;
	
};
