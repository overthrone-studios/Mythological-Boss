// Copyright Overthrone Studios 2019

#include "MainPlayerHUD.h"
#include "WidgetTree.h"
#include "ProgressBar.h"
#include "Public/OverthroneGameInstance.h"

void UMainPlayerHUD::Init()
{
	Super::Init();

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());

	PlayerHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Health"));
	PlayerStaminaBar = Cast<UProgressBar>(WidgetTree->FindWidget("Player_Stamina"));
	BossHealthBar = Cast<UProgressBar>(WidgetTree->FindWidget("Boss_Health"));
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
