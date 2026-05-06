// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_StateMachineComponent.h"
#include "Shared/Components/HitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Net/UnrealNetwork.h"

AToyOgre_Monster::AToyOgre_Monster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 카메라에 보이는지에 따라 애니메이션 업데이트 결정
	// 보스는 항상으로
	GetMesh()->VisibilityBasedAnimTickOption =
	EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	AIControllerClass = nullptr;
	AutoPossessAI = EAutoPossessAI::Disabled;
	MonsterMoveType = EMonsterMoveType::End;
	
	HitBoxComponent = CreateDefaultSubobject<UHitBoxComponent>(FName("HitBoxComponent"));
	HitBoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info HitCompInfo(
		FName("Monster"), 
		FName("MonsterWeapon"), 
		FVector(110.0f, 0.f, 150.0f), 
		FVector(220.f, 150.0f, 150.f));
	
	HitBoxComponent->InitializeHitComp(HitCompInfo, GetTargetName());
	HitBoxComponent->SetDamage(30);
	HitBoxComponent->CollisionOff();
	
	// 상태 머신 추가
	StateMachine = CreateDefaultSubobject<UToyOgre_StateMachineComponent>(TEXT("StateMachine"));
}

void AToyOgre_Monster::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		StateMachine->Init(this);
		StateMachine->ChangeState(SelectTargetStateClass);
	}
}

void AToyOgre_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString StateStr = UEnum::GetValueAsString(ToyOgreState);
	const FString LogStr = FString::Printf(TEXT("State : %s"), *StateStr);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, LogStr, nullptr, FColor::White, 0, true, 1);

	
	if (HasAuthority())
	{
		StateMachine->TickState(DeltaTime);
	}
}

void AToyOgre_Monster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AToyOgre_Monster, ToyOgreState);
}

void AToyOgre_Monster::OnRep_ToyOgreState()
{

}

void AToyOgre_Monster::SetToyOgreState(EToyOgreState NewState)
{
	if (!HasAuthority())
		return;

	if (ToyOgreState == NewState)
		return;

	ToyOgreState = NewState;

	OnRep_ToyOgreState();
}

void AToyOgre_Monster::PlayToyOgreMontage(UAnimMontage* Montage)
{
	if (!HasAuthority())
		return;

	Multicast_PlayToyOgreMontage(Montage);
}

void AToyOgre_Monster::Multicast_PlayToyOgreMontage_Implementation(UAnimMontage* Montage)
{
	if (!Montage || !GetMesh())
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst)
		return;

	AnimInst->Montage_Play(Montage);
}

void AToyOgre_Monster::AnimNotify_ToyOgre(FName EventName)
{
	if (!HasAuthority())
		return;

	if (StateMachine)
	{
		StateMachine->HandleAnimNotify(EventName);
	}
}


bool AToyOgre_Monster::RotateToCurrentTarget(float DeltaTime, float RotateSpeed)
{
	TWeakObjectPtr<AActor> Target = GetCurrentTarget();
	if (!Target.IsValid())
		return false;

	const FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	const FRotator TargetRot = FRotationMatrix::MakeFromX(ToTarget).Rotator();

	const FRotator NewRot = FMath::RInterpTo(
		GetActorRotation(),
		FRotator(0.f, TargetRot.Yaw, 0.f),
		DeltaTime,
		RotateSpeed
	);

	SetActorRotation(NewRot);
	return true;
}