// Copyright Overthrone Studios 2019

#include "MainPlayerHUD.h"

#include "OverthroneGameState.h"
#include "OverthroneFunctionLibrary.h"

#include "WidgetTree.h"

#include "ProgressBar.h"
#include "TextBlock.h"
#include "Image.h"

#include "WidgetAnimation.h"

#include "TimerManager.h"

void UMainPlayerHUD::Init()
{
	Super::Init();

	GameState = UOverthroneFunctionLibrary::GetGameState(this);

	PlayerHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Health"));
	PlayerStaminaBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Stamina"));
	BossHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Boss_Health"));

	ChargeInputKeyWidget = Cast<UUserWidget>(WidgetTree->FindWidget("ChargeTriggerKey"));
	DashAttackInputKeyWidget = Cast<UUserWidget>(WidgetTree->FindWidget("DashAttackKey"));
	ChargeRingGlow = Cast<UImage>(WidgetTree->FindWidget("ChargeMeterGlow"));

	DamageValueText = Cast<UTextBlock>(WidgetTree->FindWidget("DamageValue"));
}

void UMainPlayerHUD::ChangeBossHealthBarColor(const FLinearColor& InColor)
{
	CurrentBossHealth_BarColor = InColor;
}

FLinearColor UMainPlayerHUD::GetBossHealthBarColor() const
{
	return CurrentBossHealth_BarColor;
}

void UMainPlayerHUD::ShowDashAttackPrompt()
{
	DashAttackInputKeyWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainPlayerHUD::HideDashAttackPrompt()
{
	DashAttackInputKeyWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainPlayerHUD::ShowChargeInputKeyWidget()
{
	ChargeInputKeyWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainPlayerHUD::HideChargeInputKeyWidget()
{
	ChargeInputKeyWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainPlayerHUD::UpdateDamageValue(const float DamageAmount) const
{
	DamageValueText->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));
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

void UMainPlayerHUD::FlashStaminaBar()
{
	PlayAnimation(LowStaminaFlash, 0.0f, 0);
}

void UMainPlayerHUD::StopStaminaBarFlash()
{
	StopAnimation(LowStaminaFlash);
}

void UMainPlayerHUD::FlashHealthBar()
{
	PlayAnimation(LowHealthFlash, 0.0f, 0);
}

void UMainPlayerHUD::StopHealthBarFlash()
{
	StopAnimation(LowHealthFlash);
}

void UMainPlayerHUD::FlashChargeRing()
{
	ChargeRingGlow->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(ChargeMeterFlash, 0.0f, 0);
}

void UMainPlayerHUD::StopChargeRingFlash()
{
	ChargeRingGlow->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(ChargeMeterFlash);
}

void UMainPlayerHUD::SetOffTimer_DamageValueFadeOut(const float InSeconds)
{
	bDamageValueAnimWantsFadeIn = false;
	bIsDamageValueAnimFadingOut = false;

	GetWorld()->GetTimerManager().SetTimer(DamageValueFadeTimer, this, &UMainPlayerHUD::FadeOutDamageValue, InSeconds);
}

float UMainPlayerHUD::GetPlayerHealthAsPercentage() const
{
	return GameState->PlayerData.Health / GameState->PlayerData.StartingHealth;
}

float UMainPlayerHUD::GetPlayerSmoothedHealthAsPercentage() const
{
	return GameState->PlayerData.SmoothedHealth / GameState->PlayerData.StartingHealth;
}

float UMainPlayerHUD::GetPlayerStaminaAsPercentage() const
{
	return GameState->PlayerData.Stamina / GameState->PlayerData.StartingStamina;
}

float UMainPlayerHUD::GetPlayerSmoothedStaminaAsPercentage() const
{
	return GameState->PlayerData.SmoothedStamina / GameState->PlayerData.StartingStamina;
}

float UMainPlayerHUD::GetPlayerChargeAsPercentage() const
{
	return GameState->PlayerData.Charge / GameState->PlayerData.MaxCharge;
}

float UMainPlayerHUD::GetPlayerSmoothedChargeAsPercentage() const
{
	return GameState->PlayerData.SmoothedCharge / GameState->PlayerData.MaxCharge;
}

float UMainPlayerHUD::GetBossHealthAsPercentage() const
{
	return GameState->BossData.Health / GameState->BossData.StartingHealth;
}

float UMainPlayerHUD::GetBossSmoothedHealthAsPercentage() const
{
	return GameState->BossData.SmoothedHealth / GameState->BossData.StartingHealth;
}

void UMainPlayerHUD::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	bIsDamageValueAnimFadingOut = false;
}
