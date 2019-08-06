// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeathSignature);

/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

	FOnBossDeathSignature OnBossDeath;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		bool IsGamePaused();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void UnPauseGame();

	// Switches input mode to UI only
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void SetInputModeUI() const;

	// Switches input mode to Game only
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void SetInputModeGame() const;

	void InitInstance();

	void SetLockOnLocation(const FVector& LockOnLocation) const;
	void SetLockOnRotation(const FRotator& LockOnRotation) const;
	void ToggleLockOnVisibility(bool bIsVisible) const;

	UPROPERTY(BlueprintReadOnly, Category = "Ylva")
		FVector PlayerLocation{};

	UPROPERTY(BlueprintReadWrite, Category = "Ylva")
		float PlayerStartingHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Ylva")
		float PlayerHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Ylva")
		float PlayerStartingStamina{};

	UPROPERTY(BlueprintReadOnly, Category = "Ylva")
		float PlayerStamina{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		FVector BossLocation{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float BossHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float BossStartingHealth{};

	// The parry window time frame
	UPROPERTY(BlueprintReadOnly, Category = "Ylva")
		uint8 bParrySucceeded : 1;

	class ALockOn* LockOn;

	class AYlva* Player;
	class AMordath* Boss;

private:
	class UUserWidget* PauseMenu{};
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	APlayerController* PlayerController{};
};
