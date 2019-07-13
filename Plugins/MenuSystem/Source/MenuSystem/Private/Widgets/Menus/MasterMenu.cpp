// Copyright Ali El Saleh 2019

#include "MasterMenu.h"
#include "WidgetTree.h"
#include "MenuHUD.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

void UMasterMenu::Init()
{
	Super::Init();

	// Get the widget switcher
	WidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget(FName("MenuSwitcher")));

	Menus = GetAllChildMenus();

	for (auto Menu : Menus)
	{
		Menu->Init();
		ULog::LogDebugMessage(INFO, FString(Menu->GetName() + " initialized"), true);
	}
}

UMenuBase* UMasterMenu::GetMenu(const TSubclassOf<UMenuBase> MenuClass) const
{
	for (auto Menu : Menus)
	{
		if (Menu->IsA(MenuClass))
			return Menu;
	}

	return nullptr;
}

TArray<UMenuBase*> UMasterMenu::GetMenus() const
{
	return Menus;
}

UWidget* UMasterMenu::GetCurrentMenuWidget() const
{
	return WidgetSwitcher->GetActiveWidget();
}

int32 UMasterMenu::GetCurrentMenuIndex() const
{
	return WidgetSwitcher->GetActiveWidgetIndex();
}

void UMasterMenu::SwitchToMenu(UMenuBase* Menu)
{
	WidgetSwitcher->SetActiveWidget(Menu);
}

void UMasterMenu::SwitchToMenuIndex(const int32 Index)
{
	WidgetSwitcher->SetActiveWidgetIndex(Index);
}

TArray<UMenuBase*> UMasterMenu::GetAllChildMenus() const
{
	TArray<UMenuBase*> FoundMenus;

	for (int32 i = 0; i < WidgetSwitcher->GetChildrenCount(); i++)
	{
		const auto Menu = WidgetSwitcher->GetChildAt(i);

		if (Menu->IsA(UMenuBase::StaticClass()))
			FoundMenus.Add(Cast<UMenuBase>(Menu));
		else
			ULog::LogDebugMessage(WARNING, FString(Menu->GetName() + " is not a UMenuBase. Remove " + Menu->GetName() + " from " + GetName()), true);
	}

	return FoundMenus;
}