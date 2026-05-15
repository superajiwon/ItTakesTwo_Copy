
#include "Actors/Characters/Monsters/WeaponMonsterBase/WeaponMonsterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/InGameHPBar.h"


AWeaponMonsterBase::AWeaponMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique(TEXT("SectionCull"));
	Tags.AddUnique(TEXT("SectionCullCollision"));
	Tags.AddUnique(TEXT("SectionCullTick"));
	RightHand_WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponMeshComponent"));
	RightHand_WeaponMeshComponent->SetupAttachment(GetMesh(), FName(TEXT("RightHandSocket")));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerArrow(TEXT("/Script/Engine.StaticMesh'/Game/Models/Characters/ArrowDecal/PlayerArrow_Mesh/StaticMeshes/SM_MonsterArrow.SM_MonsterArrow'"));
	if (PlayerArrow.Succeeded()) PlayerArrowComp->SetStaticMesh(PlayerArrow.Object);
	
	HPUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HPUIComp->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> HPWidgetAsset(TEXT("/Game/UI/Blueprints/WBP_InGameHPBar.WBP_InGameHPBar_C"));
	if (HPWidgetAsset.Succeeded()) HPUIComp->SetWidgetClass(HPWidgetAsset.Class);
	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 50.0f;
	HPUIComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetDrawSize(FVector2D(50.0f, 10.0f));
	
}

void AWeaponMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	
	HPUIComp->InitWidget(); 
	InitHPBar();
	
	if (UInGameHPBar* HPBarWidget = Cast<UInGameHPBar>(GetHPUIComponent()->GetWidget()))
	{
		HPBarWidget->SetColors(FLinearColor::Red);
	}
}

void AWeaponMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


