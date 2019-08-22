// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Widgets/HUD/HUDBase.h"
#include "MainPlayerHUD.generated.h"

/**
 * The main HUD the player will see when playing the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMainPlayerHUD final : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerHealthAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerSmoothedHealthAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerStaminaAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerSmoothedStaminaAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerChargeAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetPlayerSmoothedChargeAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetBossHealthAsPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Main Player HUD")
		float GetBossSmoothedHealthAsPercentage() const;

protected:
	class UOverthroneGameInstance* GameInstance;

private:
	class UProgressBar* PlayerHealthBar;
	class UProgressBar* PlayerStaminaBar;
	class UProgressBar* BossHealthBar;
};
