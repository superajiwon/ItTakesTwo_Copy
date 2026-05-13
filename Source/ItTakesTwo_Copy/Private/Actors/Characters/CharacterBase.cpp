
#include "Actors/Characters/CharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/HPComponent.h"
#include "Components/StatComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHPBar.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 액터 자체의 복제 활성화
	bReplicates = true;
	
	HPComp = CreateDefaultSubobject<UHPComponent>(TEXT("HPComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	
	HPUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HPUIComp->SetupAttachment(GetMesh());
	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetAsset(TEXT("/Game/UI/Blueprints/WBP_InGameHPBar.WBP_InGameHPBar_C"));
	if (HPWidgetAsset.Succeeded()) HPUIComp->SetWidgetClass(HPWidgetAsset.Class);
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2 + 50.0f;
	HPUIComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetDrawSize(FVector2D(200.0f, 15.0f));
	HPUIComp->SetDrawAtDesiredSize(true);
	
	PlayerArrowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerArrowComp"));
	PlayerArrowComp->SetupAttachment(GetMesh());
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	HPUIComp->InitWidget(); 
	
	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(HPUIComp->GetWidget()))
	{
		HPBarWidget->UpdateHP(HPComp->GetCurHP(), HPComp->GetMaxHP());
		HPComp->OnHPChanged.AddDynamic(HPBarWidget, &UInGameHPBar::UpdateHP);
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// PrintNetLog();
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::Heal(float HealAmount)
{
	IHealable::Heal(HealAmount);
	
	GetHPComponent()->ApplyHeal(HealAmount);
}

void ACharacterBase::Damage(float DamageAmount, AActor* Causer)
{
	IDamagable::Damage(DamageAmount, Causer);
	
	GetHPComponent()->ApplyDamage(DamageAmount, Causer);
	
}

void ACharacterBase::PlayCamShake(float Scale)
{
	if (CamShake)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->ClientStartCameraShake(CamShake, Scale);
		}
		else if (HasAuthority())
		{
			Multicast_PlayCamShake(Scale);
		}
	}
}

void ACharacterBase::Multicast_PlayCamShake_Implementation(float Scale)
{
	if (CamShake)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->StartCameraShake(CamShake, Scale);
		}
	}
}

void ACharacterBase::PrintNetLog()
{
	const FString LogStr = FString::Printf(TEXT("HP : %.0f"), GetHPComponent()->GetCurHP());
	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}
