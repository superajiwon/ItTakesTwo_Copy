
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSkillList.generated.h"

class UPlayerSkillSlot;
class UPlayerUltimateExpBar;

UCLASS()
class ITTAKESTWO_COPY_API UPlayerSkillList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta=(BindWidget))
	UPlayerSkillSlot* Skill_Base;
	UPROPERTY(meta=(BindWidget))
	UPlayerSkillSlot* Skill_Dash;
	UPROPERTY(meta=(BindWidget))
	UPlayerSkillSlot* Skill_Special;
	UPROPERTY(meta=(BindWidget))
	UPlayerSkillSlot* Skill_Ultimate;
	
	UPROPERTY(meta=(BindWidget))
	UPlayerUltimateExpBar* Bar_UltimateExpBar;
};
