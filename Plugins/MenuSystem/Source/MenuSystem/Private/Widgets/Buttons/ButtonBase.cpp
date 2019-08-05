// Copyright Ali El Saleh 2019

#include "ButtonBase.h"
#include "MenuHUD.h"
#include "MenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetTree.h"
#include "TextBlock.h"
#include "Log.h"

void UButtonBase::Init()
{
	// Get the Menu HUD
	MenuHUD = Cast<AMenuHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	TextWidget = Cast<UTextBlock>(WidgetTree->FindWidget("Text"));

	bIsFocusable = true;
}

void UButtonBase::NativePreConstruct()
{
	TextWidget = Cast<UTextBlock>(WidgetTree->FindWidget("Text"));
	TextWidget->SetText(ButtonText);
}

void UButtonBase::OnButtonReleased()
{
	IsMenuHUDNull();

	UGameplayStatics::PlaySound2D(GetWorld(), PressedSound);
}

void UButtonBase::OnButtonHovered()
{
	HighlightText();

	if (IsMenuNull())
		return;

	Menu->SetMenuTooltipText(ButtonTooltipText);

	UGameplayStatics::PlaySound2D(GetWorld(), HoveredSound);
}

void UButtonBase::OnButtonUnhovered()
{
	UnHighlightText();

	if (IsMenuNull())
		return;

	Menu->SetMenuTooltipText(FText());
}

void UButtonBase::HighlightText()
{
	if (IsTextWidgetNull())
		return;

	TextWidget->SetOpacity(HoveredOpacity);
}

void UButtonBase::UnHighlightText()
{
	if (IsTextWidgetNull())
		return;

	TextWidget->SetOpacity(UnhoveredOpacity);
}

bool UButtonBase::IsMenuHUDNull() const
{
	if (!MenuHUD)
	{
		ULog::DebugMessage(ERROR, FString(GetName() + " | MenuHUD is null"), true);
		return true;
	}

	return false;
}

bool UButtonBase::IsMenuNull() const
{
	if (!Menu)
	{
		ULog::DebugMessage(ERROR, FString(GetName() + " | Menu is null"), true);
		return true;
	}

	return false;
}

bool UButtonBase::IsTextWidgetNull() const
{
	if (!TextWidget)
	{
		ULog::DebugMessage(ERROR, FString(GetName() + " | TextWidget is null"), true);
		return true;
	}

	return false;
}