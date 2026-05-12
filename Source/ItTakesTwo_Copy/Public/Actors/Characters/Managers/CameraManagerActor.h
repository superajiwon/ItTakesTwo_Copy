
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManagerActor.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ITTAKESTWO_COPY_API ACameraManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraManagerActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="SharedCamera")
	void AddTarget(AActor* Target);
	
	UFUNCTION(BlueprintCallable, Category="SharedCamera")
	void RemoveTarget(AActor* Target);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	UCameraComponent* Camera;
	
	// 현재 추적 중인 타겟 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shared Camera")
	TArray<AActor*> TrackTargets;
	
	// 최대 줌 아웃 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shared Camera")
	float MaxZoomDist = 2500.0f;
	// 최소 줌 아웃 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shared Camera")
	float MinZoomDist = 2000.0f;
	
	// 최소 줌 아웃 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shared Camera")
	float ZoomOutStartDist = 3000.0f;
	
	// 에디터에서 설정 가능한 던전 이탈 방지용 투명 박스
	// 카메라 중심점이 이 박스를 벗어나지 못하도록 Clamp 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	// AVolume* CameraLimitVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	bool bUseCameraLimit = false;

	// 던전 이탈 방지 최소 한계 좌표 (X, Y, Z)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera", meta = (EditCondition = "bUseCameraLimit"))
	FVector CameraLimitMin = FVector(-10000.f, -10000.f, -10000.f);

	// 던전 이탈 방지 최대 한계 좌표 (X, Y, Z)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera", meta = (EditCondition = "bUseCameraLimit"))
	FVector CameraLimitMax = FVector(10000.f, 10000.f, 10000.f);
	
	
	// 카메라 이동 부드러움 정도 (값이 클수록 빠름)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	float CameraMoveSpeed = 5.f;
	
	// 카메라 줌 부드러움 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	float CameraZoomSpeed = 3.f;
	
	// 쿼터뷰 기본 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	FRotator DefaultCameraRotation = FRotator(-45.f, -45.f, 0.f);
	
	// 카메라 회전 부드러움 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared Camera")
	float CameraRotationSpeed = 3.f;

	// 목표 회전 각도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shared Camera")
	FRotator TargetCameraRotation;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Shared Camera")
	void SetTargetCameraRotation(FRotator NewRotation);

private:
	void UpdateCameraPosition(float DeltaTime);
};
