// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "MayCharacter.generated.h"

UCLASS()
class ITTAKESTWO_COPY_API AMayCharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AMayCharacter();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// === Weapon ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SwordComp;
	
	// =========================================================
	//  May 전용: 궁극기 폼 체인지 상태
	//  Replicated → 서버에서 변경하면 클라이언트로 자동 동기화
	// =========================================================
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat|State")
	bool bIsUltimateForm = false;
	
	// =========================================================
	//  override: bIsUltimateForm 상태에 따라
	//   false → NormalAttackData    (3콤보, 스페셜 A)
	//   true  → UltimateAttackData  (1콤보, 스페셜 B)
	// =========================================================
	virtual FAttackModeData* GetCurrentAttackData() override;

	// =========================================================
	//  override: 서버에서 Ultimate(ActionType==EActionType::Ultimate) 수신 시 호출
	//  bIsUltimateForm을 서버에서 토글 → Replicated로 클라이언트 동기화
	// =========================================================
	virtual void OnUltimateActivated() override;
};
