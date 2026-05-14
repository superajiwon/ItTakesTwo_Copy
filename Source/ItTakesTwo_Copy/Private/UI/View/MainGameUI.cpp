
#include "UI/View/MainGameUI.h"

#include "UI/View/PlayerSkillList.h"
#include "UI/View/PlayerSkillSlot.h"
#include "UI/View/PlayerUltimateExpBar.h"
#include "UI/View/PlayerReviveBar.h"
#include "UI/View/PlayerPortrait.h"
#include "UI/ViewModel/HUDViewModel.h"
#include "UI/UIManager/UIMangerSubsystem.h"

void UMainGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	UTexture2D* KeyBase = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Mouse_LMB.PC_Mouse_LMB'"));
	UTexture2D* KeyDash = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Spacebar.PC_Spacebar'"));
	UTexture2D* KeySpec = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Mouse_RMB.PC_Mouse_RMB'"));
	UTexture2D* KeyUlti = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Q.PC_Q'"));
	if (KeyBase && KeyDash && KeySpec && KeyUlti)
	{
		UE_LOG(LogTemp, Warning, TEXT("Texture created"));
	}
	
	UTexture2D* MayBase = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityA1.BruteAbilityA1'"));
	if (MayBase) SkillList_May->Skill_Base->SetImages(KeyBase, MayBase);
	UTexture2D* MayDash = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityB2.BruteAbilityB2'"));
	if (MayDash) SkillList_May->Skill_Dash->SetImages(KeyDash, MayDash);
	UTexture2D* MaySpec = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityC1.BruteAbilityC1'"));
	if (MaySpec) SkillList_May->Skill_Special->SetImages(KeySpec, MaySpec);
	UTexture2D* MayUlti = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityC2.BruteAbilityC2'"));
	if (MayUlti) SkillList_May->Skill_Ultimate->SetImages(KeyUlti, MayUlti);
	
	UTexture2D* CodyBase = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityA1.MageAbilityA1'"));
	if (CodyBase) SkillList_Cody->Skill_Base->SetImages(KeyBase, CodyBase);
	UTexture2D* CodyDash = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityB2.MageAbilityB2'"));
	if (CodyDash) SkillList_Cody->Skill_Dash->SetImages(KeyDash, CodyDash);
	UTexture2D* CodySpec = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityC1.MageAbilityC1'"));
	if (CodySpec) SkillList_Cody->Skill_Special->SetImages(KeySpec, CodySpec);
	UTexture2D* CodyUlti = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityD2.MageAbilityD2'"));
	if (CodyUlti) SkillList_Cody->Skill_Ultimate->SetImages(KeyUlti, CodyUlti);
	// ViewModel Binding
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UUIMangerSubsystem* UIManager = GI->GetSubsystem<UUIMangerSubsystem>())
		{
			if (UHUDViewModel* VM = UIManager->GetHUDViewModel())
			{
				VM->OnHUDHPChanged.AddDynamic(this, &UMainGameUI::OnHPChanged);
				VM->OnHUDSkillCooldownChanged.AddDynamic(this, &UMainGameUI::OnSkillCooldownChanged);
				VM->OnHUDUltimateGaugeChanged.AddDynamic(this, &UMainGameUI::OnUltimateGaugeChanged);
				VM->OnHUDReviveTimeChanged.AddDynamic(this, &UMainGameUI::OnReviveTimeChanged);
			}
		}
	}
}

void UMainGameUI::NativeDestruct()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UUIMangerSubsystem* UIMgr = GI->GetSubsystem<UUIMangerSubsystem>())
		{
			if (UHUDViewModel* VM = UIMgr->GetHUDViewModel())
			{
				VM->OnHUDHPChanged.RemoveDynamic(this, &UMainGameUI::OnHPChanged);
				VM->OnHUDSkillCooldownChanged.RemoveDynamic(this, &UMainGameUI::OnSkillCooldownChanged);
				VM->OnHUDUltimateGaugeChanged.RemoveDynamic(this, &UMainGameUI::OnUltimateGaugeChanged);
				VM->OnHUDReviveTimeChanged.RemoveDynamic(this, &UMainGameUI::OnReviveTimeChanged);
			}
		}
	}
	
	Super::NativeDestruct();
}

void UMainGameUI::OnHPChanged(const FHUDPlayerInfo& PlayerInfo)
{
	if (PlayerInfo.Role == EPlayerRole::May &&Portrait_May)
	{
		Portrait_May->UpdateHP(PlayerInfo.CurHP, PlayerInfo.MaxHP);
	}
	else if (PlayerInfo.Role == EPlayerRole::Cody &&Portrait_Cody)
	{
		Portrait_Cody->UpdateHP(PlayerInfo.CurHP, PlayerInfo.MaxHP);
	}
}

void UMainGameUI::OnSkillCooldownChanged(EPlayerRole PlayerRole, EActionType ActionType, bool bIsOnCooldown, float CooldownTime)
{
	UPlayerSkillList* TargetList = (PlayerRole == EPlayerRole::May) ? SkillList_May : SkillList_Cody;
	if (!TargetList) return;

	switch (ActionType)
	{
	case EActionType::Basic:
		if (TargetList->Skill_Base) TargetList->Skill_Base->UpdateSkillCoolTime(bIsOnCooldown, CooldownTime);
		break;
	case EActionType::Dash:
		if (TargetList->Skill_Dash) TargetList->Skill_Dash->UpdateSkillCoolTime(bIsOnCooldown, CooldownTime);
		break;
	case EActionType::Special:
		if (TargetList->Skill_Special) TargetList->Skill_Special->UpdateSkillCoolTime(bIsOnCooldown, CooldownTime);
		break;
	case EActionType::Ultimate:
		if (TargetList->Skill_Ultimate) TargetList->Skill_Ultimate->UpdateSkillCoolTime(bIsOnCooldown, CooldownTime);
		break;
	default:
		break;
	}
}

void UMainGameUI::OnUltimateGaugeChanged(EPlayerRole PlayerRole, float CurGauge, float MaxGauge)
{
	UPlayerSkillList* TargetList = (PlayerRole == EPlayerRole::May) ? SkillList_May : SkillList_Cody;
	if (TargetList && TargetList->Bar_UltimateExpBar)
	{
		TargetList->Bar_UltimateExpBar->UpdateGauge(CurGauge, MaxGauge);
	}
}

void UMainGameUI::OnReviveTimeChanged(EPlayerRole PlayerRole, bool bIsDead, float ReviveTime)
{
	if (PlayerRole == EPlayerRole::May && ReviveBar_May)
	{
		if (bIsDead)
		{
			ReviveBar_May->SetVisibility(ESlateVisibility::Visible);
			PlayAnimation(GettingSmallMay);
		}
		else
		{
			ReviveBar_May->SetVisibility(ESlateVisibility::Hidden);
		}
		ReviveBar_May->StartRevive(bIsDead, ReviveTime);
	}
	else if (PlayerRole == EPlayerRole::Cody && ReviveBar_Cody)
	{
		if (bIsDead)
		{
			ReviveBar_Cody->SetVisibility(ESlateVisibility::Visible);
			PlayAnimation(GettingSmallCody);
		}
		else
		{
			ReviveBar_Cody->SetVisibility(ESlateVisibility::Hidden);
		}
		ReviveBar_Cody->StartRevive(bIsDead, ReviveTime);
	}
}
