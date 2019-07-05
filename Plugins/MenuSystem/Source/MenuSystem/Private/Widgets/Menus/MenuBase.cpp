// Copyright Ali El Saleh 2019

#include "MenuBase.h"
#include "MenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "ButtonBase.h"
#include "TimerManager.h"
#include "Debug.h"
#include "WidgetTree.h"
#include "MenuSetting.h"
#include "WidgetAnimation.h"

void UMenuBase::Init()
{
	MenuHUD = Cast<AMenuHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	if (!Animation)
		UDebug::LogDebugMessage(ERROR, FString(GetName() + " | Fade anim is null. You forgot to assign the fade animation in widget blueprint"), true);
}

void UMenuBase::InitializeSettings()
{
	for (auto Setting : MenuSettings)
	{
		//Setting->Init();
		//Setting->SetMenuReference(this);
	}
}

void UMenuBase::InitializeButtons()
{
	ParentBox = Cast<UPanelWidget>(WidgetTree->FindWidget("MenuOptions"));

	//for (auto Button : ParentBox->GetAllChildren())
	//{
	//	Cast<UButtonBase>(Button)->Init();
	//}
}

void UMenuBase::AddSetting(UMenuSetting* Setting)
{
	MenuSettings.Add(Setting);
}

void UMenuBase::StoreAllSettings(UPanelWidget* ParentWidget)
{
	//for (auto Widget : ParentWidget->GetAllChildren())
	//{
	//	AddSetting(Cast<UMenuSetting>(Widget));
	//}
}

void UMenuBase::FadeIn()
{
	if (Animation)
		PlayAnimation(Animation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UMenuBase::FadeOut()
{
	if (Animation)
		PlayAnimation(Animation);
}

void UMenuBase::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMenuBase::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (RenderOpacity <= 0.0f)
		SetVisibility(ESlateVisibility::Hidden);
}

void UMenuBase::Apply()
{
	//for (auto MenuSetting : MenuSettings)
	//{
	//	MenuSetting->Apply();
	//}

	GEngine->GetGameUserSettings()->ApplySettings(false);
}

void UMenuBase::Forward(const EButtonType Menu)
{
	MenuSelected = Menu;
	GetWorld()->GetTimerManager().SetTimer(ForwardTimerHandle, this, &UMenuBase::GoForward, 1.0f, false, Animation->GetEndTime());
}

void UMenuBase::GoForward()
{
	GetWorld()->GetTimerManager().ClearTimer(ForwardTimerHandle);
}

void UMenuBase::Back()
{
	GetWorld()->GetTimerManager().SetTimer(BackTimerHandle, this, &UMenuBase::GoBack, 1.0f, false, Animation->GetEndTime());
}

void UMenuBase::GoBack()
{
	GetWorld()->GetTimerManager().ClearTimer(BackTimerHandle);
}

void UMenuBase::SetMenuTooltipText(const FText& Text)
{
	MenuTooltipText = Text;
}

