
#include "UI/View/FloatingUI.h"
#include "Components/TextBlock.h"

void UFloatingUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFloatingUI::PlayFloating(FText Text, FLinearColor OutLinearColor)
{	
	ShowText->SetText(Text);
	FSlateFontInfo FontInfo = ShowText->GetFont();
	FontInfo.OutlineSettings.OutlineColor = OutLinearColor;
	FontInfo.OutlineSettings.OutlineSize = 2;
	ShowText->SetFont(FontInfo);
	
	PlayAnimation(Floating);
}
