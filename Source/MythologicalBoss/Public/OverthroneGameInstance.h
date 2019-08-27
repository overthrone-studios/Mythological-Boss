// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowStaminaSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSecondStageSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThirdStageSignature);

USTRUCT()
struct FCharacterInfo
{
	GENERATED_BODY()

	FVector Location{};

	float StartingHealth = 0.0f;
	float Health = 0.0f;
	float SmoothedHealth = 0.0f;

	FOnLowHealthSignature OnLowHealth;

	uint8 bIsDead : 1;
};

USTRUCT()
struct FPlayerInfo : public FCharacterInfo
{
	GENERATED_BODY()

	FOnLowStaminaSignature OnLowStamina;

	float MaxCharge = 0.0f;
	float StartingStamina = 0.0f;
	float Stamina = 0.0f;
	float SmoothedStamina = 0.0f;
	float Charge = 0.0f;
	float SmoothedCharge = 0.0f;

	float TeleportRadius = 0.0f;

	uint8 bParrySucceeded : 1;
};

/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

	FOnBossDeathSignature OnPlayerDeath;
	FOnBossDeathSignature OnBossDeath;
	FOnSecondStageSignature OnSecondStage;
	FOnThirdStageSignature OnThirdStage;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void RestartGame();

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

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		FORCEINLINE float GetTeleportRadius() const { return PlayerInfo.TeleportRadius; };

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		class UFeatData* GetFeat(const FString& FeatName);

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void ResetFeats();

	void InitFeats();
	void InitInstance();

	void SetLockOnLocation(const FVector& LockOnLocation) const;
	void SetLockOnRotation(const FRotator& LockOnRotation) const;
	void ToggleLockOnVisibility(bool bIsVisible) const;

	FBox PlayArea{};

	FPlayerInfo PlayerInfo;
	FCharacterInfo BossInfo;

	class ALockOn* LockOn;

	class AYlva* Player;
	class AMordath* Boss;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Instance")
		TArray<class UFeatData*> Feats;

	UFUNCTION()
		void OnFeatAchieved();

	class UFeatData* AchievedFeat;

private:
	uint8 bFirstLaunch : 1;

	class UUserWidget* PauseMenu{};
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	APlayerController* PlayerController{};
};
