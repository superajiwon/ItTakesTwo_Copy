// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Actor/CullingSection_EntranceBox.h"

#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/Respawn_TargetPoint.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "ITTGameMode.h"
#include "Actors/Characters/Managers/CameraManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Shared/Actor/CullingBoundBoxManager.h"


ACullingSection_EntranceBox::ACullingSection_EntranceBox()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique(TEXT("NoSectionCull"));

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	TriggerBox->SetCollisionProfileName(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetGenerateOverlapEvents(true);
	
}

void ACullingSection_EntranceBox::BeginPlay()
{
	Super::BeginPlay();
	
	if (!CullingManager)
	{
		for (TActorIterator<ACullingBoundBoxManager> It(GetWorld()); It; ++It)
		{
			CullingManager = *It;
			break;
		}
	}

	if (TriggerBox)
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACullingSection_EntranceBox::OnTriggerBeginOverlap);
	
	// 배치되어있는 Respawn_TargetPoint의 인덱스가 이 EntranceBox의 인덱스와 같은 경우에 추가 
	for (TActorIterator<ARespawn_TargetPoint> It(GetWorld()); It; ++It)
	{
		ARespawn_TargetPoint* TargetPoint = *It;
		if (TargetPoint->GetIndex() == SectionIndex)
		{
			if (TargetPoint->IsCodyRespawnPoint())
				CurRespawnPointCody = TargetPoint;
			else
				CurRespawnPointMay = TargetPoint;
		}
	}
}

void ACullingSection_EntranceBox::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	if (!OtherActor->ActorHasTag(PlayerTag))
		return;

	if (!CullingManager)
		return;

	if (SectionIndex < 0)
		return;
	
	if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
	{
		CullingManager->PlayerSectionChange(Player, SectionIndex);
		
		if (HasAuthority())
		{
			if (AITTGameMode* GM = Cast<AITTGameMode>(GetWorld()->GetAuthGameMode()))
			{
				GM->UpdateRespawnPoint(CurRespawnPointCody, CurRespawnPointMay);
			}
		}
				
		if (bChangeCameraRotation)
		{
			AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManagerActor::StaticClass());
			if (ACameraManagerActor* CamManager = Cast<ACameraManagerActor>(FoundCamera))
			{
				CamManager->SetTargetCameraRotation(TargetCameraRotation);
			}
		}
	}
}

