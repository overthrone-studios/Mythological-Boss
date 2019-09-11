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

	UFUNCTION(BlueprintCallable, Category = "Main Player HUD")
		void UpdateDamageValue(float DamageAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Main Player HUD")
		void FadeInDamageValue();

	UFUNCTION(BlueprintCallable, Category = "Main Player HUD")
		void FadeOutDamageValue();

	UFUNCTION(BlueprintCallable, Category = "Main Player HUD")
		void SetOffTimer_DamageValueFadeOut(float InSeconds);

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

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		class UWidgetAnimation* DamageValueFade;

	void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	class UProgressBar* PlayerHealthBar;
	class UProgressBar* PlayerStaminaBar;
	class UProgressBar* BossHealthBar;
	class UTextBlock* DamageValueText;

	FTimerHandle DamageValueFadeTimer;

	uint8 bDamageValueAnimWantsFadeIn : 1;
	uint8 bIsDamageValueAnimFadingOut : 1;
};
