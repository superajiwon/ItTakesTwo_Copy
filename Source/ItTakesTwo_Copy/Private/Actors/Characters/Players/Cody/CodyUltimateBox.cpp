
#include "Actors/Characters/Players/Cody/CodyUltimateBox.h"
#include "Actors/Characters/Players/PlayerBase.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/UltimateComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shared/Components/DotHitBoxComponent.h"
#include "Shared/Struct/HitComp_Info.h"

ACodyUltimateBox::ACodyUltimateBox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true); // 서버에서의 Attach 정보를 클라이언트에도 동기화하기 위해 필수!
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	CenterComp = CreateDefaultSubobject<USceneComponent>("CenterComp");
	CenterComp->SetupAttachment(Root);
	
	AttackBox = CreateDefaultSubobject<UDotHitBoxComponent>("AttackBox");
	AttackBox->SetupAttachment(CenterComp);
	AttackBox->SetRelativeLocation(FVector(0.5f, 0.0f, 0.0f));
	AttackBox->SetBoxExtent(FVector(0.5f, 50, 50));
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
	
	BeamLengthParam = FName("User.BeamLength"); // 로그에 None이 뜨지 않도록 기본값 지정
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
	if (!GetAttachParentActor()) return;
	
	FVector S = GetActorLocation();
	FVector E = S + GetActorForwardVector() * MaxDistance;
	FHitResult Hit;
	FCollisionQueryParams Params; 	
	Params.AddIgnoredActor(this); 
	Params.AddIgnoredActor(GetAttachParentActor()); // 부모(코디) 무조건 무시
	if (GetOwner()) Params.AddIgnoredActor(GetOwner()); // 서버에서 세팅된 오너 무시
	
	// 1. 콜리전 응답 파라미터를 하나 만들어 줍니다!
	FCollisionResponseParams ResponseParams;
	// 2. 대장님의 커스텀 채널(예: 채널 1)을 Ignore로 덮어씌웁니다!
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel1, ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel2, ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel3, ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel4, ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel8, ECR_Ignore);
	
	// 3. 함수의 맨 마지막 6번째 인자로 ResponseParams를 넘깁니다!
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, S, E, ECC_Visibility, Params, ResponseParams);
	
	// bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, S, E, ECC_Visibility, Params);
	if (bHit)
	{	// 벽에 맞은 경우: 히트 거리를 목표로 설정
		TargetDistance = Hit.Distance;
		Impact->SetWorldLocation(Hit.ImpactPoint);
		Impact->SetVisibility(true);
		if (!Impact->IsActive()) Impact->Activate();
	}
	else
	{
		// 벽이 없는 경우: MaxDistance까지 뻗어나가도록 설정
		TargetDistance = MaxDistance;
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
	
	UE_LOG(LogTemp, Warning, TEXT("%s, Distance: %f"), *BeamLengthParam.ToString(), Distance);
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
		
		AttackBox->CollisionOn();
	}
}

void ACodyUltimateBox::CollisionOff()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		bIsBeamActive = false;
		
		OnRep_IsBeamActive(); // ★ 서버/호스트에서도 빔을 끄기 위해 반드시 직접 호출해야 합니다!
		
		AttackBox->CollisionOff();
	}
}

void ACodyUltimateBox::OnRep_IsBeamActive()
{
	if (bIsBeamActive)
	{	// 빔 시작: Distance를 0에서부터 Lerp로 뻗어나가도록 초기화
		Distance = 0.0f;
		TargetDistance = 0.0f;
		Beam->SetFloatParameter(BeamLengthParam, 0.0f);
		Beam->SetVisibility(true);
		if (!Beam->IsActive()) Beam->Activate();
	}
	else
	{	// 빔 종료: 모든 VFX 비활성화
		Beam->SetVisibility(false);
		Beam->Deactivate();
		Impact->SetVisibility(false);
		Impact->Deactivate();
		Distance = 0.0f;
		TargetDistance = 0.0f;
	}
}

 