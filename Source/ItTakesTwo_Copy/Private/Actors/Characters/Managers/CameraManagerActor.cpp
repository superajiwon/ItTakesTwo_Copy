

#include "Actors/Characters/Managers/CameraManagerActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Characters/Players/PlayerBase.h"
#include "Components/HPComponent.h"

ACameraManagerActor::ACameraManagerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;
	SpringArm->bDoCollisionTest = false; // 지형과 부딪히지 않도록
	SpringArm->TargetArmLength = MinZoomDist;
	SpringArm->bEnableCameraLag = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ACameraManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
	SpringArm->SetRelativeRotation(DefaultCameraRotation);
	
	// 플레이어들을 한 번만 찾아서 등록
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundPlayers);
	for (AActor* Player : FoundPlayers)
	{
		AddTarget(Player);
	}
}

void ACameraManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateCameraPosition(DeltaTime);
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->IsLocalController() && PC->GetViewTarget() != this)
		{
			PC->SetViewTargetWithBlend(this, 0.f);
		}
	}
}

void ACameraManagerActor::AddTarget(AActor* Target)
{
	if (Target == nullptr || TrackTargets.Contains(Target)) return;
	
	TrackTargets.Add(Target);
}

void ACameraManagerActor::RemoveTarget(AActor* Target)
{
	if (Target == nullptr) return;
	
	TrackTargets.Remove(Target);
}

void ACameraManagerActor::UpdateCameraPosition(float DeltaTime)
{
	// 방장(호스트)이나 클라이언트가 AddTarget을 개별적으로 호출하면서 생기는 싱크/네트워크 꼬임을 완벽하게 방지.
	// 기존에 추가된 미니보스 등의 타겟을 날리지 않기 위해 AddUnique만 수행.
	// TArray<AActor*> FoundPlayers;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundPlayers);
	// for (AActor* Player : FoundPlayers)
	// {
	// 	APlayerBase* PlayerBase = Cast<APlayerBase>(Player);
	// 	if (PlayerBase && !PlayerBase->GetHPComponent()->GetIsDead())
	// 	{
	// 		TrackTargets.AddUnique(Player);	
	// 	}
	// 	else
	// 	{
	// 		TrackTargets.Remove(Player);
	// 	}
	// }
	//! 성능 최적화를 위해 BeginPlay에서 한번만 실행
	
	if (TrackTargets.Num() == 0) return;
	
	// 타겟들의 위치를 모두 합산할 변수
	FVector Centroid = FVector::ZeroVector;
	// 실제 월드에 존재하는(소멸되지 않은) 유효한 타겟의 수
	int32 ValidTargetCount = 0;
	
	// 타겟들이 분포한 가장 끝단(최소/최대 좌표)을 찾기 위한 초기화
	FVector MinLocation = FVector(MAX_flt, MAX_flt, MAX_flt);
	FVector MaxLocation = FVector(-MAX_flt, -MAX_flt, -MAX_flt);

	// 2. 등록된 모든 타겟(메이, 코디, 미니보스 등)을 순회하며 위치 정보를 수집합니다.
	for (AActor* Target : TrackTargets)
	{
		// 유효한 액터인지 검사 (중간에 파괴된 액터가 있을 수 있으므로 방어 코드)
		if (Target && IsValid(Target))
		{			
			// 플레이어나 캐릭터인 경우, 죽은 상태라면 카메라 계산에서 제외 (포커싱 방지)
			if (ACharacterBase* Character = Cast<ACharacterBase>(Target))
			{
				if (Character->GetHPComponent() && Character->GetHPComponent()->GetIsDead())
				{
					continue; 
				}
			}
			
			FVector Loc = Target->GetActorLocation();
			
			// 평균점을 구하기 위해 모든 위치를 더해줍니다.
			Centroid += Loc;
			ValidTargetCount++;
			
			// 가장 작은 X, Y, Z (바운딩 박스의 좌하단) 갱신
			MinLocation.X = FMath::Min(MinLocation.X, Loc.X);
			MinLocation.Y = FMath::Min(MinLocation.Y, Loc.Y);
			MinLocation.Z = FMath::Min(MinLocation.Z, Loc.Z);

			// 가장 큰 X, Y, Z (바운딩 박스의 우상단) 갱신
			MaxLocation.X = FMath::Max(MaxLocation.X, Loc.X);
			MaxLocation.Y = FMath::Max(MaxLocation.Y, Loc.Y);
			MaxLocation.Z = FMath::Max(MaxLocation.Z, Loc.Z);
		}
	}

	// 순회 결과 유효한 타겟이 하나도 없었다면 연산을 중단합니다.
	if (ValidTargetCount == 0) return;
	
	// 3. 타겟들의 평균 위치(초기 중심점)를 구합니다.
	Centroid /= ValidTargetCount;

	// 4. 타겟 간 이격 거리를 계산합니다. (가장 멀리 떨어진 두 점의 거리)
	// 이 거리는 카메라를 얼마나 줌아웃 할지 결정하는 척도가 됩니다.
	float TargetDistance = FVector::Distance(MinLocation, MaxLocation);
	
	// 5. 줌 인/아웃 거리를 결정합니다.
	// 현재 플레이어 간 거리를 최대 한계 거리(MaxTargetDistanceForZoom)로 나누어 0.0 ~ 1.0 사이의 비율(ZoomFactor)을 구합니다.
	float ZoomFactor = FMath::Clamp(TargetDistance / ZoomOutStartDist, 0.f, 1.f);
	
	// 계산된 비율에 따라 최소 줌(가장 가까울 때)과 최대 줌(가장 멀 때) 사이를 선형 보간(Lerp)합니다.
	float TargetArmLength = FMath::Lerp(MinZoomDist, MaxZoomDist, ZoomFactor);

	// [참고] 만약 플레이어들이 특정 한계 거리 이상 벌어진 경우(ZoomFactor가 1에 근접한 경우)
	// 중심점을 단순 평균이 아닌, 던전을 앞서 나가는 플레이어(진행 중인 타겟) 쪽으로 
	// 가중치(Weight)를 주어 포커싱을 몰아주는 로직을 이곳에 추가할 수 있습니다.

	/*
	// 6. 던전 이탈 방지 볼륨 경계 제한 (Volume Clamp) 적용
	// 에디터 상에 설정된 투명 박스(CameraLimitVolume)가 있다면 중심점이 그 박스를 벗어나지 못하도록 가둡니다.
	if (CameraLimitVolume && CameraLimitVolume->GetBrushComponent())
	{
		// 볼륨의 바운딩 박스 크기를 가져옵니다.
		FBox VolumeBounds = CameraLimitVolume->GetBrushComponent()->Bounds.GetBox();
		
		// 중심점 좌표가 볼륨의 Min, Max를 절대 넘어갈 수 없게 강제로 잘라냅니다(Clamp).
		Centroid.X = FMath::Clamp(Centroid.X, VolumeBounds.Min.X, VolumeBounds.Max.X);
		Centroid.Y = FMath::Clamp(Centroid.Y, VolumeBounds.Min.Y, VolumeBounds.Max.Y);
		// Z축(높이)도 고정하고 싶다면 동일하게 처리합니다.
		Centroid.Z = FMath::Clamp(Centroid.Z, VolumeBounds.Min.Z, VolumeBounds.Max.Z);
	}
	*/
	
	// 6. 던전 이탈 방지 경계 제한 (Clamp) 적용
	// 직접 입력한 최소/최대 좌표(CameraLimitMin, CameraLimitMax)를 바탕으로 화면을 가둡니다.
	if (bUseCameraLimit)
	{
		// 중심점 좌표가 입력된 Min, Max를 절대 넘어갈 수 없게 강제로 잘라냅니다(Clamp).
		Centroid.X = FMath::Clamp(Centroid.X, CameraLimitMin.X, CameraLimitMax.X);
		Centroid.Y = FMath::Clamp(Centroid.Y, CameraLimitMin.Y, CameraLimitMax.Y);
		// Z축(높이) 제한의 경우
		Centroid.Z = FMath::Clamp(Centroid.Z, CameraLimitMin.Z, CameraLimitMax.Z);
	}
	
	// 7. 카메라의 부드러운 위치 이동 (댐핑 처리)
	// 현재 위치에서 목표 위치(Centroid)로 순간이동하지 않고, DeltaTime과 MoveSpeed를 곱해 부드럽게 쫓아갑니다.
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, Centroid, DeltaTime, CameraMoveSpeed);
	SetActorLocation(NewLocation);

	// 8. 카메라의 부드러운 줌 아웃/인 (거리 댐핑 처리)
	// 스프링암의 길이 역시 설정된 줌 속도(CameraZoomSpeed)에 맞춰 부드럽게 변환시킵니다.
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, CameraZoomSpeed);
}

