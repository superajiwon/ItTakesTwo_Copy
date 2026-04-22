// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/MayCharacter.h"
#include "Actors/Characters/Players/PlayerActionData.h"
#include "Net/UnrealNetwork.h"

AMayCharacter::AMayCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/May_Castle/SkeletalMeshes/May_Castle.May_Castle'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	
	SwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	SwordComp->SetupAttachment(GetMesh(), FName(TEXT("RightAttachSocket")));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/Characters/May_Castle/May_Sword/StaticMeshes/May_Sword.May_Sword'"));
	if (TempSwordMesh.Succeeded())
		SwordComp->SetStaticMesh(TempSwordMesh.Object);
}

void AMayCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// bIsUltimateForm을 모든 클라이언트에 복제
	DOREPLIFETIME(AMayCharacter, bIsUltimateForm);
}

FAttackModeData* AMayCharacter::GetCurrentAttackData()
{
	if (!ActionData) return nullptr;
	
	if (bIsUltimateForm && ActionData->UltimateAttackData.BasicAttackCombos.Num() > 0)
		return &ActionData->UltimateAttackData;
	
	return &ActionData->NormalAttackData;
}

// Server_ExecuteSkill_Implementation에서 서버 단독 호출
// bIsUltimateForm이 Replicated이므로 변경 시 클라이언트로 자동 동기화
void AMayCharacter::OnUltimateActivated()
{
	// 서버에서만 호출되므로 HasAuthority() 체크 불필요
	bIsUltimateForm = !bIsUltimateForm;
}
