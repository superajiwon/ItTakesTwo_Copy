
#include "UI/View/MainGameUI.h"

#include "UI/View/PlayerSkillList.h"
#include "UI/View/PlayerSkillSlot.h"

void UMainGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	ConstructorHelpers::FObjectFinder<UTexture2D> KeyBase(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Mouse_LMB.PC_Mouse_LMB'"));
	ConstructorHelpers::FObjectFinder<UTexture2D> KeyDash(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Spacebar.PC_Spacebar'"));
	ConstructorHelpers::FObjectFinder<UTexture2D> KeySpec(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Mouse_RMB.PC_Mouse_RMB'"));
	ConstructorHelpers::FObjectFinder<UTexture2D> KeyUlti(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/PC_Q.PC_Q'"));
	if (KeyBase.Succeeded() && KeyDash.Succeeded() && KeySpec.Succeeded() && KeyUlti.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Material created"));
	}
	
	ConstructorHelpers::FObjectFinder<UTexture2D> MayBase(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityA1.BruteAbilityA1'"));
	if (MayBase.Succeeded()) SkillList_May->Skill_Base->SetImages(KeyBase.Object, MayBase.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> MayDash(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityB2.BruteAbilityB2'"));
	if (MayDash.Succeeded()) SkillList_May->Skill_Dash->SetImages(KeyDash.Object, MayDash.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> MaySpec(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityC1.BruteAbilityC1'"));
	if (MayDash.Succeeded()) SkillList_May->Skill_Special->SetImages(KeySpec.Object, MaySpec.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> MayUlti(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/BruteAbilityC2.BruteAbilityC2'"));
	if (MayDash.Succeeded()) SkillList_May->Skill_Ultimate->SetImages(KeyUlti.Object, MayUlti.Object);
	
	ConstructorHelpers::FObjectFinder<UTexture2D> CodyBase(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityA1.MageAbilityA1'"));
	if (MayBase.Succeeded()) SkillList_Cody->Skill_Base->SetImages(KeyBase.Object, CodyBase.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> CodyDash(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityB2.MageAbilityB2'"));
	if (MayDash.Succeeded()) SkillList_Cody->Skill_Dash->SetImages(KeyDash.Object, CodyDash.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> CodySpec(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityC1.MageAbilityC1'"));
	if (MayDash.Succeeded()) SkillList_Cody->Skill_Special->SetImages(KeySpec.Object, CodySpec.Object);
	ConstructorHelpers::FObjectFinder<UTexture2D> CodyUlti(TEXT("/Script/Engine.Texture2D'/Game/UI/Img/MageAbilityD2.MageAbilityD2'"));
	if (MayDash.Succeeded()) SkillList_Cody->Skill_Ultimate->SetImages(KeyUlti.Object, CodyUlti.Object);
}
