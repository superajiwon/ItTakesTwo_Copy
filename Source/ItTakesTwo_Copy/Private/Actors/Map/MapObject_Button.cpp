// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapObject_Button.h"

#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"


AMapObject_Button::AMapObject_Button()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(true);
	MeshComp->SetCollisionProfileName("MapObject");
	MeshComp->SetupAttachment(RootComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionProfileName("Button");
	BoxComponent->SetupAttachment(RootComponent);
	
}

void AMapObject_Button::BeginPlay()
{
	Super::BeginPlay();

}
void AMapObject_Button::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!HasAuthority())
		return;
	
	if (CurrentState == EMapObjectState::Active)
		return;

	if (Cast<APlayerBase>(OtherActor))
	{
		if (PlayerCount == 0)
		{
			if (UGameInstance* GameInstance = GetGameInstance())
			{
				if (USoundManagerSubsystem* SoundManager =
					GameInstance->GetSubsystem<USoundManagerSubsystem>())
				{
					SoundManager->PlaySFX3D(TEXT("SFX_ButtonPress"), GetActorLocation());
				}
			}
		}

		CurrentState = EMapObjectState::AfterPressed;
		++PlayerCount;
		PlayerCount = FMath::Clamp(PlayerCount, 0, 2);
	}
}
void AMapObject_Button::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!HasAuthority())
		return;
	
	if (CurrentState == EMapObjectState::Active)
		return;
	if (Cast<APlayerBase>(OtherActor))
	{
		--PlayerCount;
		PlayerCount = FMath::Clamp(PlayerCount, 0, 2);
		if (PlayerCount == 0)
			CurrentState = EMapObjectState::BeforePressed;
	}
	
}

void AMapObject_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

