// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/PlayerBase.h"
#include "Actors/Characters/Players/ITTPlayerController.h"

#include "Components/SkillComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerBase::APlayerBase()
{
	// === Component ===
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));
	
	// === Input ===
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_PlayerMapping.IMC_PlayerMapping'"));
	if (TempIMC.Succeeded()) IMC_PlayerMapping = TempIMC.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded()) IA_Move = TempMoveInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempBaseInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_BaseAttack.IA_BaseAttack'"));
	if (TempBaseInput.Succeeded()) IA_BaseAttack = TempBaseInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempSpecInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SpecialAttack.IA_SpecialAttack'"));
	if (TempSpecInput.Succeeded()) IA_SpecialAttack = TempSpecInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempDashInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Dash.IA_Dash'"));
	if (TempDashInput.Succeeded()) IA_Dash = TempDashInput.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> TempUltiInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Ultimate.IA_Ultimate'"));
	if (TempUltiInput.Succeeded()) IA_Ultimate = TempUltiInput.Object;
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APlayerBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto* PC = Cast<AITTPlayerController>(GetController());
	if (PC && PC->IsLocalPlayerController())
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem) Subsystem->AddMappingContext(IMC_PlayerMapping, 0);
		
		auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (PlayerInput)
		{
			PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerBase::Move);
			PlayerInput->BindAction(IA_BaseAttack, ETriggerEvent::Started, this, &APlayerBase::BaseAttack);
			PlayerInput->BindAction(IA_SpecialAttack, ETriggerEvent::Started, this, &APlayerBase::SpecialAttack);
			PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &APlayerBase::Dash);
			PlayerInput->BindAction(IA_Ultimate, ETriggerEvent::Started, this, &APlayerBase::Ultimate);  
		}
	}
}

void APlayerBase::Move(const FInputActionValue& Value)
{if (!Controller) return;

	FVector2D V = Value.Get<FVector2D>(); // X: 좌우, Y: 상하 (일반적인 IA_Move 설정 기준)

	const FRotator Rotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 카메라 기준 정면과 오른쪽 방향 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, V.X);
	AddMovementInput(RightDirection, V.Y);
}

void APlayerBase::BaseAttack(const FInputActionValue& Value)
{
	
}

void APlayerBase::SpecialAttack(const FInputActionValue& Value)
{
	
}

void APlayerBase::Dash(const FInputActionValue& Value)
{
	
}

void APlayerBase::Ultimate(const FInputActionValue& Value)
{
	
}
