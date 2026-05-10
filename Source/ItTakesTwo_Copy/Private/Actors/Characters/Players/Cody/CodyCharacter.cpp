
#include "Actors/Characters/Players/Cody/CodyCharacter.h"

#include "Actors/Characters/Players/Cody/CodyUltimateBox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shared/VFXObjectPoolSubsystem.h"
#include "Shared/Components/HitSphereComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

ACodyCharacter::ACodyCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Characters/Cody_Wizard/SkeletalMeshes/Cody_Wizard.Cody_Wizard'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	
	AttackColliderPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttackColliderPoint"));
	AttackColliderPoint->SetupAttachment(GetCapsuleComponent());
	AttackColliderPoint->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	
	BaseCollision = CreateDefaultSubobject<UHitSphereComponent>(TEXT("BaseCollision"));
	BaseCollision->AttachToComponent(AttackColliderPoint, FAttachmentTransformRules::KeepRelativeTransform);
	FHitComp_Info BasicHitCompInfo(FName("Player_CodyBasic"), FName("PlayerWeapon"), FVector(250.0f,0.0f,0.0f), 200.f);
	BaseCollision->InitializeHitComp(BasicHitCompInfo, GetTargetName());
	BaseCollision->CollisionOff();
	
	SpecialProjectilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpecialProjectilePoint"));
	SpecialProjectilePoint->SetupAttachment(AttackColliderPoint);
	SpecialProjectilePoint->SetRelativeLocation(FVector(70.0f,0.0f,125.0f));
	
	// === Infinite 나이아가라 컴포넌트 ===
	HandNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HandNiagaraComp"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HandNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_Cody_Always.NS_Cody_Always'"));
	if (HandNiagaraAsset.Succeeded()) HandNiagaraComp->SetAsset(HandNiagaraAsset.Object);
	HandNiagaraComp->SetupAttachment(GetMesh(), FName("RightNiagaraSocket"));
	HandNiagaraComp->SetAutoActivate(true); // 항상 켜져있음
}

void ACodyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에 UltimateBox 액터를 스폰 (서버에서만 스폰해야 중복 생성을 막을 수 있습니다)
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; 
		UltimateCollision = GetWorld()->SpawnActor<ACodyUltimateBox>(ACodyUltimateBox::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
		
		if (UltimateCollision)
		{
			UltimateCollision->AttachToComponent(SpecialProjectilePoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void ACodyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACodyCharacter, UltimateCollision);
}

void ACodyCharacter::OnRep_UltimateCollision()
{
	if (UltimateCollision)
	{
		// 클라이언트에서도 완벽하게 부착되도록 강제합니다
		UltimateCollision->AttachToComponent(SpecialProjectilePoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACodyCharacter::SetWeaponCollision(bool bEnable)
{
	if (!BaseCollision) return;

	if (bEnable)
	{
		// Debug
		BaseCollision->SetHiddenInGame(false);
		int32 RandDamage = FMath::RandRange(5, 18);
		BaseCollision->SetDamage(RandDamage);
		BaseCollision->CollisionOn();
	}
	else
	{
		// Debug
		BaseCollision->SetHiddenInGame(true);
		BaseCollision->CollisionOff();
	}
}

void ACodyCharacter::SpecialAttack(const FInputActionValue& Value)
{
	Super::SpecialAttack(Value);
	
}

void ACodyCharacter::CodyTeleport(float Distance)
{
	FVector StartLocation = GetActorLocation();
	FVector DistLocation = StartLocation + (GetActorForwardVector() * Distance);
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bCanTeleport = false; 
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), DistLocation, ECC_Visibility, Params);
	if (bHit && Hit.GetActor())
	{
		bCanTeleport = Hit.GetActor()->ActorHasTag(FName("CanTeleport"));
	}
	
	SetActorLocation(DistLocation, !bCanTeleport); // bSweep

	// 이동 완료 후 도착 지점에 VFX 재생 (서버에서 호출 → Multicast로 전파)
	Multicast_PlayTeleportVFX(GetActorLocation());
	
	// bNoCheck = true : 철창이나 좁은 틈새를 무시하고 통과하기 위해 '체크 안 함' 설정! 
	// TeleportTo는 막혀있어도 그걸 뚫고 지나가기 때문에 사용하면 안된다 
	// TeleportTo(DistLocation, GetActorRotation(), false, bCanTeleport); 
}

void ACodyCharacter::Server_CodyTeleport_Implementation()
{
	CodyTeleport(TeleportLength);
}

void ACodyCharacter::Ultimate(const FInputActionValue& Value)
{
	Super::Ultimate(Value);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACodyCharacter::EndUltimate()
{
	Super::EndUltimate();
	
	if (UltimateCollision)
	{
		// UltimateCollision->SetHiddenInGame(true);
		UltimateCollision->CollisionOff();
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACodyCharacter::CancelUltimateOnAction(EActionType ActionType)
{
	 if (ActionType == EActionType::Dash)
	 {
		 Super::CancelUltimateOnAction(ActionType);
	 }
}

void ACodyCharacter::Multicast_PlayBaseAttackVFX_Implementation()
{
	if (!BaseAttackVFX) return;

	// 손 소켓 위치에 스폰 (HandNiagaraComp의 위치와 동일)
	const FVector SpawnLocation = BaseCollision->GetComponentLocation();
	const FRotator SpawnRotation = BaseCollision->GetComponentRotation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BaseAttackVFX, SpawnLocation, SpawnRotation);
}

void ACodyCharacter::Multicast_PlayTeleportVFX_Implementation(FVector Location)
{
	if (!TeleportVFX) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TeleportVFX, Location, GetActorRotation());
}

