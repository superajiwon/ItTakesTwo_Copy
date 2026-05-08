
#include "Actors/Characters/Players/Cody/CodyUltimateBox.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

ACodyUltimateBox::ACodyUltimateBox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	CenterComp = CreateDefaultSubobject<USceneComponent>("CenterComp");
	CenterComp->SetupAttachment(Root);
	
	AttackBox = CreateDefaultSubobject<UDotHitBoxComponent>("AttackBox");
	AttackBox->SetupAttachment(CenterComp);
	AttackBox->CollisionOff();
	
	Beam = CreateDefaultSubobject<UNiagaraComponent>("Beam");
	Beam->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BeamNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_CodyBeam.NS_CodyBeam'"));
	if (BeamNiagaraAsset.Succeeded()) Beam->SetAsset(BeamNiagaraAsset.Object);
	Beam->SetVisibility(false);
	
	Impact = CreateDefaultSubobject<UNiagaraComponent>("Impact");
	Impact->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ImpactNiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/VFX/Using/NS_CodyBeam_Impact.NS_CodyBeam_Impact'"));
	if (ImpactNiagaraAsset.Succeeded()) Impact->SetAsset(ImpactNiagaraAsset.Object);
	Impact->SetVisibility(false);
}

void ACodyUltimateBox::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto* OwnerPlayer = Cast<APlayerBase>(GetOwner()))
	{
		FHitComp_Info SwordHitCompInfo(
			FName("Player_CodyUltimate"), 
			FName("PlayerWeapon"), 
			FVector(0.5f, 0.0f, 0.0f), 
			FVector(0.5f, 50, 50));
		AttackBox->InitializeHitComp(SwordHitCompInfo, OwnerPlayer->GetTargetName());
	}
}

void ACodyUltimateBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsBeamActive) return;
	
	FVector S = GetActorLocation();
	FVector E = S + GetActorForwardVector() * MaxDistance;
	FHitResult Hit;
	FCollisionQueryParams Params; 
	Params.AddIgnoredActor(GetOwner());
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, S, E, ECC_Visibility, Params);
	
	if (bHit)
	{
		TargetDistance = Hit.Distance;
		Impact->SetWorldLocation(Hit.ImpactPoint);
		Impact->SetVisibility(true);
		if (!Impact->IsActive()) Impact->Activate();
	}
	else
	{
		TargetDistance = Hit.Distance;
		Impact->SetVisibility(false);
		Impact->Deactivate();
	}
	
	// Lerp
	Distance = FMath::FInterpTo(Distance, TargetDistance, DeltaTime, LerpSpeed);
	
	if (GetOwner()->HasAuthority())
	{
		CenterComp->SetRelativeScale3D(FVector(Distance, 1, 1));
	}
	else
	{
		CenterComp->SetRelativeScale3D(FVector(Distance, 1, 1));
	}
	
	Beam->SetVectorParameter(BeamLengthParam, FVector(Distance, 0.0f, 0.0f));
}

void ACodyUltimateBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACodyUltimateBox, bIsBeamActive);
}

void ACodyUltimateBox::SetDamage(int32 Damage)
{
	AttackBox->SetDamage(Damage);
}

void ACodyUltimateBox::CollisionOn()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		bIsBeamActive = true;
		
		OnRep_IsBeamActive(); // Call for server/host
		
		// todo SetHiddenInGame 나중에 지워야함 
		AttackBox->SetHiddenInGame(false);
		AttackBox->CollisionOn();
	}
}

void ACodyUltimateBox::CollisionOff()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		bIsBeamActive = false;
		
		// todo SetHiddenInGame 나중에 지워야함 
		AttackBox->SetHiddenInGame(true);
		AttackBox->CollisionOff();
	}
}

void ACodyUltimateBox::OnRep_IsBeamActive()
{
	if (bIsBeamActive)
	{
		Distance = 0.0f;
	}
}

 