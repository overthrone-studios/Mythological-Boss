// Copyright Ali El Saleh 2019

#include "MenuSetting.h"
#include "MenuHUD.h"
#include "MenuBase.h"
#include "Engine/Engine.h"
#include "WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

void UMenuSetting::Init()
{
	MenuHUD = Cast<AMenuHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	GameUserSettings = GEngine->GetGameUserSettings();
}

void UMenuSetting::Reset()
{
	GameUserSettings->ApplySettings(false);
}

void UMenuSetting::SetMenuReference(UMenuBase* InMenu)
{
	Menu = InMenu;
}

void UMenuSetting::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (!Menu)
	{
		ULog::LogDebugMessage(ERROR, FString("Menu is null. Menu reference has not been set in the menu's Init function"), true);
		return;
	}

	Menu->SetMenuTooltipText(SettingTooltipText);
}

void UMenuSetting::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (!Menu)
	{
		ULog::LogDebugMessage(ERROR, FString("Menu is null. Menu reference has not been set in the menu's Init function"), true);
		return;
	}

	Menu->SetMenuTooltipText(FText());
}

void UMenuSetting::Apply()
{
	GameUserSettings->ApplySettings(false);
}