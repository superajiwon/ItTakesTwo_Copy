// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/MenuController.h"

#include "Shared/ITTSessionSubsystem.h"

void AMenuController::BeginPlay()
{
	Super::BeginPlay();

}

void AMenuController::HostStart()
{
	UITTSessionSubsystem* SessionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UITTSessionSubsystem>();
	SessionSubsystem->CreateSession(4);
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


void AMenuController::Server_SelectCharacter_Implementation(EPlayerRole SelectedRole)
{
	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	// 버튼이 눌렸는지 확인하기 위한 로그 출력
	FString RoleStr = (SelectedRole == EPlayerRole::May) ? TEXT("May") : (SelectedRole == EPlayerRole::Cody) ? TEXT("Cody") : TEXT("None");
	UE_LOG(LogTemp, Warning, TEXT("[Server_SelectCharacter] Button Clicked! Requested Role: %s"), *RoleStr);

	// 역할을 중복 선택하지 못하게 방어
	if (IsLocalPlayerController()) // 서버 입장에서는 자기가 호스트인지 체크
	{
		// 호스트가 선택하는 경우
		if (GI->ClientSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 클라이언트가 이미 선점한 캐릭터를 골랐으므로 무시
			return;
		}
		GI->HostSelectedRole = SelectedRole;
	}
	else
	{
		// 클라이언트가 선택하는 경우
		if (GI->HostSelectedRole == SelectedRole && SelectedRole != EPlayerRole::None)
		{
			// 호스트가 이미 선점했으므로 무시
			return;
		}
		GI->ClientSelectedRole = SelectedRole;
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
	// 이 함수는 양쪽 PC에서 각자 실행됩니다.
	// 블루프린트로 구현된 UI 업데이트 노드(OnSelectionUpdated)를 실행시킵니다.
	OnSelectionUpdated(HostRole, ClientRole);
}

void AMenuController::Server_StartGame_Implementation()
{
	UITTGameInstance* GI = GetGameInstance<UITTGameInstance>();
	if (!GI) return;

	// 양쪽 플레이어가 모두 캐릭터를 골랐을 때만 맵을 넘깁니다.
	if (GI->HostSelectedRole != EPlayerRole::None && GI->ClientSelectedRole != EPlayerRole::None)
	{
		// 서버와 클라이언트 모두 던전 맵으로 이동 (listen 옵션 포함)
		// GetWorld()->ServerTravel("Lv_Dungeon?listen");
		GetWorld()->ServerTravel("Lv_ServerTestInGame?listen");
	}
}
