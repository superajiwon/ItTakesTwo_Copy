// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/MenuController.h"

#include "EngineUtils.h"
#include "Shared/ITTSessionSubsystem.h"
#include "Shared/Subsystems/SoundManagerSubsystem.h"
#include "Actors/Characters/Players/Rose/RoseCharacter.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AMenuController::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->bEnableOnScreenDebugMessages = false;
	}
	UKismetSystemLibrary::ExecuteConsoleCommand(
	GetWorld(),
	TEXT("DisableAllScreenMessages"),
	this
	);
	if (IsLocalController())
	{
		if (USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>())
		{
			SoundManager->PlayBGM(TEXT("BGM_MainMenu"));
		}
		
		// 레벨에 배치된 CameraActor를 찾습니다.
		TArray<AActor*> FoundCameras;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCameras);
		if (FoundCameras.Num() > 0)
		{
			// 찾은 첫 번째 카메라로 시점을 고정합니다.
			SetViewTargetWithBlend(FoundCameras[0]);
		}
	}
	
	if (UITTSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UITTSessionSubsystem>())
	{
		SessionSubsystem->TrySendPendingInvite();
	}
}

void AMenuController::HostStart()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->CreateSession(2);
}

void AMenuController::ClientJoin()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->JoinSession();
	
}

void AMenuController::FindSession()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->FindSession();
}

void AMenuController::ExecuteServerTravel(FString MapName)
{	
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server_StartGame] ServerTravel to %s"), *MapName);
		GetWorld()->ServerTravel(MapName, true);
	}
}

void AMenuController::Server_SelectCharacter_Implementation(EPlayerRole SelectedRole)
{
	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	// 버튼이 눌렸는지 확인하기 위한 로그 출력
	FString RoleStr = (SelectedRole == EPlayerRole::Cody) ? TEXT("Cody"): (SelectedRole == EPlayerRole::May) ? TEXT("May")  : TEXT("None");
	UE_LOG(LogTemp, Warning, TEXT("[Server_SelectCharacter] Button Clicked! Requested Role: %s"), *RoleStr);

	// 역할을 중복 선택하지 못하게 방어
	if (IsLocalPlayerController()) // 서버 입장에서는 자기가 호스트인지 체크
	{
		// 호스트가 선택하는 경우
		if (GI->HostSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 자신이 이미 선택한 캐릭터를 다시 누르면 취소 (None으로 변경)
			GI->HostSelectedRole = EPlayerRole::None;
		}
		else if (GI->ClientSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 클라이언트가 이미 선점한 캐릭터를 골랐으므로 무시
			return;
		}
		else
		{
			GI->HostSelectedRole = SelectedRole;
		}
	}
	else
	{
		// 클라이언트가 선택하는 경우
		if (GI->ClientSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 자신이 이미 선택한 캐릭터를 다시 누르면 취소 (None으로 변경)
			GI->ClientSelectedRole = EPlayerRole::None;
		}
		else if (GI->HostSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 호스트가 이미 선점했으므로 무시
			return;
		}
		else
		{
			GI->ClientSelectedRole = SelectedRole;
		}
	}
	
	// 선택이 정상적으로 적용된 후 로그 출력
	RoleStr = (SelectedRole == EPlayerRole::May) ? TEXT("May") : (SelectedRole == EPlayerRole::Cody) ? TEXT("Cody") : TEXT("None");
	UE_LOG(LogTemp, Warning, TEXT("[Server_SelectCharacter] Final Selected Role: %s"), *RoleStr);

	// 갱신된 선택 현황을 모든 연결된 플레이어의 화면에 뿌려줌
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMenuController* PC = Cast<AMenuController>(It->Get());
		if (PC)
		{
			PC->Client_UpdateSelectionUI(GI->HostSelectedRole, GI->ClientSelectedRole);
		}
	}
}

void AMenuController::Client_UpdateSelectionUI_Implementation(EPlayerRole HostRole, EPlayerRole ClientRole)
{
	OnSelectionUpdated(HostRole, ClientRole);
}

void AMenuController::Server_StartGame_Implementation()
{
	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	UE_LOG(LogTemp, Warning, TEXT("[Server_StartGame] HostRole=%d ClientRole=%d"),
		static_cast<int32>(GI->HostSelectedRole),
		static_cast<int32>(GI->ClientSelectedRole));

	if (GI->HostSelectedRole != EPlayerRole::None && GI->ClientSelectedRole != EPlayerRole::None)
	{	
		// [수정된 부분] 레벨에 배치된 RoseCharacter 액터를 직접 찾아야 합니다.
		for (TActorIterator<ARoseCharacter> It(GetWorld()); It; ++It)
		{
			ARoseCharacter* Rose = *It;
			if (Rose)
			{
				Rose->Multicast_Selected(); // 서버에서 멀티캐스트 함수 호출
				break;
			}
		}

	}
}
