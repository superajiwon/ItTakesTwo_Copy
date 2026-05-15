
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
	HPUIComp->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetAsset(TEXT("/Game/UI/Blueprints/WBP_InGameHPBar.WBP_InGameHPBar_C"));
	if (HPWidgetAsset.Succeeded()) HPUIComp->SetWidgetClass(HPWidgetAsset.Class);
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 50.0f;
	HPUIComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetDrawSize(FVector2D(50.0f, 10.0f));
	
	PlayerArrowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerArrowComp"));
	PlayerArrowComp->SetupAttachment(GetMesh());
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	HPUIComp->InitWidget(); 
	InitHPBar();
	
	// if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(HPUIComp->GetWidget()))
	// {
	// 	HPBarWidget->UpdateHP(HPComp->GetCurHP(), HPComp->GetMaxHP());
	// 	HPComp->OnHPChanged.AddDynamic(HPBarWidget, &UInGameHPBar::UpdateHP);
	// }
}

// void ACharacterBase::InitHPBar()
// {
// 	if (!HPUIComp) return;
//
// 	if (HPUIComp->GetWidgetClass() == nullptr)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("[%s] HPUIComp의 WidgetClass가 None입니다! 블루프린트에서 WBP_InGameHPBar를 지정해주세요!"), *GetName());
// 	}
// 	
// 	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(HPUIComp->GetWidget()))
// 	{
// 		// 위젯이 생성되었다면 색상을 먼저 설정합니다.
// 		HPBarWidget->SetColors(HPBarColor, HPBarHitColor);
// 		
// 		// 델리게이트를 바인딩하고 현재 HP로 업데이트합니다.
// 		HPBarWidget->UpdateHP(HPComp->GetCurHP(), HPComp->GetMaxHP());
// 		HPComp->OnHPChanged.AddDynamic(HPBarWidget, &UInGameHPBar::UpdateHP);
// 	}
// 	else
// 	{
// 		// 네트워크 환경 등으로 인해 클라이언트에서 위젯 생성이 지연되는 경우, 0.1초 후 재시도합니다. (Tick 대신 Timer 사용)
// 		FTimerHandle TimerHandle;
// 		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::InitHPBar, 0.1f, false);
// 	}
// }


void ACharacterBase::InitHPBar()
{
	if (!HPUIComp) return;

	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(HPUIComp->GetWidget()))
	{
		UE_LOG(LogTemp, Warning, TEXT("==== [%s] HP바 위젯 바인딩 성공! ===="), *GetName());
		
		// 위젯이 생성되었다면 색상을 먼저 설정합니다.
		HPBarWidget->SetColors(HPBarColor, HPBarHitColor);
		
		// 델리게이트를 바인딩하고 현재 HP로 업데이트합니다.
		HPBarWidget->UpdateHP(HPComp->GetCurHP(), HPComp->GetMaxHP());
		HPComp->OnHPChanged.AddDynamic(HPBarWidget, &UInGameHPBar::UpdateHP);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] HP바 위젯 아직 생성 안됨. 0.1초 후 재시도..."), *GetName());
		// 네트워크 환경 등으로 인해 클라이언트에서 위젯 생성이 지연되는 경우, 0.1초 후 재시도합니다.
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::InitHPBar, 0.1f, false);
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
	//const FString LogStr = FString::Printf(TEXT("HP : %.0f"), GetHPComponent()->GetCurHP());
	//DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, LogStr, nullptr, FColor::White, 0, true, 1);
}
