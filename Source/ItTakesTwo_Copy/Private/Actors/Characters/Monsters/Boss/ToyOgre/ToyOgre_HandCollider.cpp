

#include "Actors/Characters/Monsters/Boss/ToyOgre/ToyOgre_HandCollider.h"

#include "Actors/Characters/Monsters/Boss/ToyOgre_Monster.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/InGameHPBar.h"
#include "UI/Actor/FloatingUIActor.h"
#include "UI/UIManager/FloatingUIPoolSubsystem.h"


AToyOgre_HandCollider::AToyOgre_HandCollider()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicateMovement(false);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);

	SphereCollision->SetSphereRadius(80.f);
	SphereCollision->SetCollisionProfileName(TEXT("MonsterBody"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetGenerateOverlapEvents(false);

	Tags.Add(TEXT("Monster"));
	
	HPUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HPUIComp->SetupAttachment(SphereCollision);
	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetAsset(TEXT("/Game/UI/Blueprints/WBP_InGameHPBar.WBP_InGameHPBar_C"));
	if (HPWidgetAsset.Succeeded()) HPUIComp->SetWidgetClass(HPWidgetAsset.Class);
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetVisibility(false);
	
	static ConstructorHelpers::FClassFinder<AFloatingUIActor> FloatingUI(TEXT("/Game/UI/Blueprints/BP_FloatingUIActor.BP_FloatingUIActor_C"));
	if (FloatingUI.Succeeded()) FloatingUIClass = FloatingUI.Class;
}

void AToyOgre_HandCollider::Damage(float DamageAmount, AActor* Causer)
{
	if (!HasAuthority() || bBroken)
		return;

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);
	OnHPChanged.Broadcast(CurrentHP, MaxHP);
	
	if (CurrentHP <= 0.f)
	{
		bBroken = true;
		DeactivateHand();

		if (OwnerOgre)
		{
			OwnerOgre->OnHandBroken(IsLeftHand);
		}
	}
	
	FLinearColor Color = FLinearColor::Red;
	if (Causer->ActorHasTag(TEXT("May")))
		Color = FLinearColor::Blue;
	if (Causer->ActorHasTag(TEXT("Cody")))
		Color = FLinearColor::Green;
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 88.0f);
	Multicast_ShowDamageUI(DamageAmount, SpawnLocation, Color);
}

void AToyOgre_HandCollider::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AToyOgre_HandCollider, CurrentHP);
	DOREPLIFETIME(AToyOgre_HandCollider, bBroken);
}
void AToyOgre_HandCollider::InitHand(AToyOgre_Monster* InOwnerOgre, bool IsLeftSide)
{
	OwnerOgre = InOwnerOgre;
	IsLeftHand = IsLeftSide;

	CurrentHP = MaxHP;
	bBroken = false;

	DeactivateHand();
}

void AToyOgre_HandCollider::ActivateHand()
{
	if (bBroken || !SphereCollision)
		return;

	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->UpdateOverlaps();	
	Multicast_SetHPBarVisible(true);
}

void AToyOgre_HandCollider::DeactivateHand()
{
	if (!SphereCollision)
		return;

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetGenerateOverlapEvents(false);
	Multicast_SetHPBarVisible(false);
}

void AToyOgre_HandCollider::RegenHand()
{
	CurrentHP = MaxHP;
	bBroken = false;
	
	OnHPChanged.Broadcast(CurrentHP, MaxHP);
	
	DeactivateHand();
}


void AToyOgre_HandCollider::BeginPlay()
{
	Super::BeginPlay();
	
	InitHPBar();
}

void AToyOgre_HandCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//
	// if (!SphereCollision)
	// 	return;
	//
	// const FVector Location = SphereCollision->GetComponentLocation();
	// const float Radius = SphereCollision->GetScaledSphereRadius();
	//
	// // 콜라이더 범위 출력
	// DrawDebugSphere(
	// 	GetWorld(),
	// 	Location,
	// 	Radius,
	// 	24,
	// 	bBroken ? FColor::Red : FColor::Green,
	// 	false,
	// 	0.f,
	// 	0,
	// 	2.f
	// );

	// HP 텍스트 출력
	// const FString DebugText = FString::Printf(
	// 	TEXT("%s Hand\nHP: %.0f / %.0f\nBroken: %s"),
	// 	IsLeftHand ? TEXT("Left") : TEXT("Right"),
	// 	CurrentHP,
	// 	MaxHP,
	// 	bBroken ? TEXT("true") : TEXT("false")
	// );
	//
	// DrawDebugString(
	// 	GetWorld(),
	// 	Location + FVector(0.f, 0.f, Radius + 50.f),
	// 	DebugText,
	// 	nullptr,
	// 	FColor::White,
	// 	0.f,
	// 	true
	// );
}

void AToyOgre_HandCollider::OnRep_CurrentHP()
{
	OnHPChanged.Broadcast(CurrentHP, MaxHP);
}

void AToyOgre_HandCollider::OnRep_bBroken()
{	
	// bBroken이 true로 복제되면 클라이언트에서도 HP바를 숨김
	if (bBroken)
	{
		if (HPUIComp) HPUIComp->SetVisibility(false);
	}
}

void AToyOgre_HandCollider::InitHPBar()
{
	if (!HPUIComp) return;

	float Height = SphereCollision->GetScaledSphereRadius() + 50.0f;
	HPUIComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0));
	HPUIComp->SetDrawSize(FVector2D(80.0f, 12.0f));
	
	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(HPUIComp->GetWidget()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("==== [%s] HP바 위젯 바인딩 성공! ===="), *GetName());
		
		HPBarWidget->SetColors(FLinearColor::Red, FLinearColor::White);
		HPBarWidget->UpdateHP(CurrentHP, MaxHP);
		
		OnHPChanged.AddDynamic(HPBarWidget, &UInGameHPBar::UpdateHP);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("[%s] HP바 위젯 아직 생성 안됨. 0.1초 후 재시도..."), *GetName());
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AToyOgre_HandCollider::InitHPBar, 0.1f, false);
	}
}

void AToyOgre_HandCollider::Multicast_SetHPBarVisible_Implementation(bool bVisible)
{
	if (HPUIComp) HPUIComp->SetVisibility(bVisible);
}

void AToyOgre_HandCollider::Multicast_ShowDamageUI_Implementation(float DamageAmount, FVector SpawnLocation, FLinearColor SpawnColor)
{
	if (UWorld* World = GetWorld())
	{
		if (UFloatingUIPoolSubsystem* PoolSubsystem = World->GetSubsystem<UFloatingUIPoolSubsystem>())
		{
			if (AFloatingUIActor* FloatingActor = PoolSubsystem->GetFloatingUIActor(FloatingUIClass, SpawnLocation))
			{
				FloatingActor->ActivateFloatingUI(FText::AsNumber(DamageAmount), SpawnColor);
			}
		}
	}
}

