// Copyright Overthrone Studios 2019

#include "MainPlayerHUD.h"

#include "Public/OverthroneGameInstance.h"

#include "WidgetTree.h"

#include "ProgressBar.h"
#include "TextBlock.h"

#include "WidgetAnimation.h"

#include "TimerManager.h"
#include "Log.h"

void UMainPlayerHUD::Init()
{
	Super::Init();

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());

	PlayerHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Health"));
	PlayerStaminaBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Stamina"));
	BossHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Boss_Health"));

	DamageValueText = Cast<UTextBlock>(WidgetTree->FindWidget("DamageValue"));
}

void UMainPlayerHUD::UpdateDamageValue(const float DamageAmount) const
{
	DamageValueText->SetText(FText::AsNumber(int32(DamageAmount)));
}

void UMainPlayerHUD::FadeInDamageValue()
{
	bDamageValueAnimWantsFadeIn = true;

	if (!IsAnimationPlaying(DamageValueFade) && DamageValueText->RenderOpacity < 1.0f || bIsDamageValueAnimFadingOut)
		PlayAnimation(DamageValueFade);

	SetOffTimer_DamageValueFadeOut(2.0f);
}

void UMainPlayerHUD::FadeOutDamageValue()
{
	if (bDamageValueAnimWantsFadeIn)
		FadeInDamageValue();
	else
	{
		bIsDamageValueAnimFadingOut = true;
		PlayAnimation(DamageValueFade, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UMainPlayerHUD::SetOffTimer_DamageValueFadeOut(const float InSeconds)
{
	bDamageValueAnimWantsFadeIn = false;
	bIsDamageValueAnimFadingOut = false;

	GetWorld()->GetTimerManager().SetTimer(DamageValueFadeTimer, this, &UMainPlayerHUD::FadeOutDamageValue, InSeconds);
}

float UMainPlayerHUD::GetPlayerHealthAsPercentage() const
{
	return GameInstance->PlayerData.Health / GameInstance->PlayerData.StartingHealth;
}

float UMainPlayerHUD::GetPlayerSmoothedHealthAsPercentage() const
{
	return GameInstance->PlayerData.SmoothedHealth / GameInstance->PlayerData.StartingHealth;
}

float UMainPlayerHUD::GetPlayerStaminaAsPercentage() const
{
	return GameInstance->PlayerData.Stamina / GameInstance->PlayerData.StartingStamina;
}

float UMainPlayerHUD::GetPlayerSmoothedStaminaAsPercentage() const
{
	return GameInstance->PlayerData.SmoothedStamina / GameInstance->PlayerData.StartingStamina;
}

float UMainPlayerHUD::GetPlayerChargeAsPercentage() const
{
	return GameInstance->PlayerData.Charge / GameInstance->PlayerData.MaxCharge;
}

float UMainPlayerHUD::GetPlayerSmoothedChargeAsPercentage() const
{
	return GameInstance->PlayerData.SmoothedCharge / GameInstance->PlayerData.MaxCharge;
}

float UMainPlayerHUD::GetBossHealthAsPercentage() const
{
	return GameInstance->BossData.Health / GameInstance->BossData.StartingHealth;
}

float UMainPlayerHUD::GetBossSmoothedHealthAsPercentage() const
{
	return GameInstance->BossData.SmoothedHealth / GameInstance->BossData.StartingHealth;
}

void UMainPlayerHUD::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	bIsDamageValueAnimFadingOut = false;
}
