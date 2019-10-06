// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OverthroneEnums.h"
#include "OverthroneGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackParryedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackBlockedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowStaminaSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstStageSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSecondStageSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThirdStageSignature);

USTRUCT()
struct FCharacterData
{
	GENERATED_BODY()

	virtual ~FCharacterData(){}

	virtual void ResetData()
	{
		StartingHealth = 0.0f;
		Health = 0.0f;
		SmoothedHealth = 0.0f;

		bIsDead = false;

		CurrentRange = BRM_Mid;
	}

	FVector Location{};

	float StartingHealth = 0.0f;
	float Health = 0.0f;
	float SmoothedHealth = 0.0f;

	FOnDeathSignature OnDeath;
	FOnLowHealthSignature OnLowHealth;

	uint8 bIsDead : 1;
	uint8 bHasTakenDamage : 1;

	EBossRange_Mordath CurrentRange = BRM_Mid;
};

USTRUCT()
struct FPlayerData : public FCharacterData
{
	GENERATED_BODY()

	virtual ~FPlayerData(){}

	void ResetData() override
	{
		Super::ResetData();

		Charge = 0.0f;
		MaxCharge = 0.0f;
		SmoothedCharge = 0.0f;
		
		Stamina = 0.0f;
		StartingStamina = 0.0f;
		SmoothedStamina = 0.0f;

		TeleportRadius = 0.0f;

		bParrySucceeded = false;
	}

	float MaxCharge = 0.0f;
	float StartingStamina = 0.0f;
	float Stamina = 0.0f;
	float SmoothedStamina = 0.0f;
	float Charge = 0.0f;
	float SmoothedCharge = 0.0f;

	float TeleportRadius = 0.0f;

	uint8 bParrySucceeded : 1;

	EAttackType_Player CurrentAttackType = ATP_None;

	FOnLowStaminaSignature OnLowStamina;
};

USTRUCT()
struct FBossData : public FCharacterData
{
	GENERATED_BODY()

	virtual ~FBossData(){}

	void ResetData() override
	{
		Super::ResetData();

		CurrentCounterType = ACM_NoCounter;
	}

	FVector LockOnBoneLocation;

	FLinearColor DamageColor = FLinearColor::White;

	EActionType_Mordath CurrentActionType;
	EAttackCounters_Mordath CurrentCounterType;

	FOnAttackParryedSignature OnAttackParryed;
	FOnAttackBlockedSignature OnAttackBlocked;

	FOnFirstStageSignature OnEnterFirstStage;
	FOnSecondStageSignature OnEnterSecondStage;
	FOnThirdStageSignature OnEnterThirdStage;
};

/**
 * Manager object for the current game state (Boss fight specifically)
 */
UCLASS()
class MYTHOLOGICALBOSS_API AOverthroneGameState final : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// Trigger a boss stage
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State | Boss")
		void EnterBossStage(EBossStage_Mordath InBossStage);

	// Restarts the current boss battle game
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void RestartGame();

	// Checks to see if the game is currently paused
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		bool IsGamePaused();

	// Pause the game and set input mode to UI
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void PauseGame();

	// UnPause the game and set input mode to Game
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void UnPauseGame();

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE APlayerCameraManager* GetCameraManager() const { return CameraManager; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE float GetTeleportRadius() const { return PlayerData.TeleportRadius; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE FVector GetBossLocation() const { return BossData.Location; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsBossDead() const { return BossData.bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsPlayerDead() const { return PlayerData.bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsPlayerAttacking() const { return PlayerData.CurrentAttackType != ATP_None; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsBossAttacking() const { return BossData.CurrentActionType != ATM_None; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool HasPlayerTakenDamage() const { return PlayerData.bHasTakenDamage; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool HasBossTakenDamage() const { return BossData.bHasTakenDamage; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE bool IsBossAttackParryable() const { return BossData.CurrentCounterType == ACM_Parryable || BossData.CurrentCounterType == ACM_ParryableBlockable; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE bool IsBossAttackBlockable() const { return BossData.CurrentCounterType == ACM_Blockable; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE bool IsBossAttackNoCounter() const { return BossData.CurrentCounterType == ACM_NoCounter; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE EActionType_Mordath GetCurrentBossAttackType() const { return BossData.CurrentActionType; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE FLinearColor GetBossDamageColor() const { return BossData.DamageColor; }

	// The player character
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Characters")
		class AYlva* Player;

	// The boss character
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Characters")
		class AMordath* Boss;

	// The lockon actor in the world
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Lock-On")
		class ALockOn* LockOn;

	// Character Data
	FPlayerData PlayerData;
	FBossData BossData;

	class UCameraShake* CurrentCameraShake;

	FBox PlayArea{};

protected:
	void BeginPlay() override;

private:
	APlayerController* PlayerController{};
	APlayerCameraManager* CameraManager{};
};
