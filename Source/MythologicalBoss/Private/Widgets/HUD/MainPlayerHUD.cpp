// Copyright Overthrone Studios 2019

#include "MainPlayerHUD.h"

#include "Public/OverthroneGameInstance.h"

#include "WidgetTree.h"

#include "ProgressBar.h"
#include "TextBlock.h"

#include "WidgetAnimation.h"

#include "TimerManager.h"

void UMainPlayerHUD::Init()
{
	Super::Init();

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());

	PlayerHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Health"));
	PlayerStaminaBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Stamina"));
	BossHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Boss_Health"));

	DamageValueText = Cast<UTextBlock>(WidgetTree->FindWidget("DamageValue"));

	//DamageValueFade->OnAnimationStarted.AddDynamic(this, &UMainPlayerHUD::OnDamageValueFadeAnimStarted);
	//DamageValueFade->OnAnimationFinished.AddDynamic(this, &UMainPlayerHUD::OnDamageValueFadeAnimFinished);
}

void UMainPlayerHUD::UpdateDamageValue(const float DamageAmount) const
{
	DamageValueText->SetText(FText::AsNumber(int32(DamageAmount)));
}

void UMainPlayerHUD::FadeInDamageValue()
{
	bWantsFadeIn = true;

	if (!IsAnimationPlaying(DamageValueFade) && DamageValueText->RenderOpacity < 1.0f || bIsFadingOut)
		PlayAnimation(DamageValueFade);
	else
		SetOffTimer_DamageValueFadeOut(2.0f);
}

void UMainPlayerHUD::FadeOutDamageValue()
{
	if (bWantsFadeIn)
		FadeInDamageValue();
	else
	{
		bIsFadingOut = true;
		PlayAnimation(DamageValueFade, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UMainPlayerHUD::SetOffTimer_DamageValueFadeOut(const float InSeconds)
{
	bWantsFadeIn = false;
	bIsFadingOut = false;

	GetWorld()->GetTimerManager().SetTimer(DamageValueFadeTimer, this, &UMainPlayerHUD::FadeOutDamageValue, InSeconds);
}

float UMainPlayerHUD::GetPlayerHealthAsPercentage() const
{
	return GameInstance->PlayerInfo.Health / GameInstance->PlayerInfo.StartingHealth;
}

float UMainPlayerHUD::GetPlayerSmoothedHealthAsPercentage() const
{
	return GameInstance->PlayerInfo.SmoothedHealth / GameInstance->PlayerInfo.StartingHealth;
}

float UMainPlayerHUD::GetPlayerStaminaAsPercentage() const
{
	return GameInstance->PlayerInfo.Stamina / GameInstance->PlayerInfo.StartingStamina;
}

float UMainPlayerHUD::GetPlayerSmoothedStaminaAsPercentage() const
{
	return GameInstance->PlayerInfo.SmoothedStamina / GameInstance->PlayerInfo.StartingStamina;
}

float UMainPlayerHUD::GetPlayerChargeAsPercentage() const
{
	return GameInstance->PlayerInfo.Charge / GameInstance->PlayerInfo.MaxCharge;
}

float UMainPlayerHUD::GetPlayerSmoothedChargeAsPercentage() const
{
	return GameInstance->PlayerInfo.SmoothedCharge / GameInstance->PlayerInfo.MaxCharge;
}

float UMainPlayerHUD::GetBossHealthAsPercentage() const
{
	return GameInstance->BossInfo.Health / GameInstance->BossInfo.StartingHealth;
}

float UMainPlayerHUD::GetBossSmoothedHealthAsPercentage() const
{
	return GameInstance->BossInfo.SmoothedHealth / GameInstance->BossInfo.StartingHealth;
}

void UMainPlayerHUD::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	bIsAnimationFinished = true;
	bIsFadingOut = false;
}
