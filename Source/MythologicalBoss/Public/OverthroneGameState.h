// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OverthroneEnums.h"
#include "OverthroneGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMordathBaseDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackParryedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackBlockedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowStaminaSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLowStaminaSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstStageSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSecondStageSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThirdStageSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerPerfectDashSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMordathDisappearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMordathReappearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMordathBeginReappearSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMordathBeginDisappearSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorEnterEnergySphereSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorExitEnergySphereSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePausedSignature, bool, bIsPaused);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLightningStrikeSpawned);


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
	FOnExitLowHealthSignature OnExitLowHealth;

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
	FOnExitLowStaminaSignature OnExitLowStamina;

	FOnPlayerPerfectDashSignature OnPlayerPerfectDash;

	uint32 LightAttacks = 0;
	uint32 HeavyAttacks = 0;
	uint32 ChargeAttacks = 0;
	uint32 DashAttacks = 0;
	uint32 Dashes = 0;
	uint32 Blocks = 0;
	uint32 Parries = 0;
};

USTRUCT()
struct FBossData : public FCharacterData
{
	GENERATED_BODY()

	virtual ~FBossData(){}

	void ResetData() override
	{
		Super::ResetData();

		CurrentCounterType = ACM_None;
	}

	FName LockOnBoneName;

	uint8 bHasAttackBegun : 1;

	FLinearColor DamageColor = FLinearColor::White;

	TArray<FVector> GhostSpawnPoints;

	EActionType_Mordath CurrentActionType;
	EAttackCounters_Mordath CurrentCounterType;
	EBossStage_Mordath CurrentStage;

	FOnAttackParryedSignature OnAttackParryed;
	FOnAttackBlockedSignature OnAttackBlocked;

	FOnFirstStageSignature OnEnterFirstStage;
	FOnSecondStageSignature OnEnterSecondStage;
	FOnThirdStageSignature OnEnterThirdStage;

	FOnActorEnterEnergySphereSignature OnActorEnterEnergySphere;
	FOnActorExitEnergySphereSignature OnActorExitEnergySphere;

	FOnMordathDisappearedSignature OnMordathDisappeared;
	FOnMordathReappearedSignature OnMordathReappeared;

	FOnMordathBeginReappearSignature OnMordathBeginReappear;
	FOnMordathBeginDisappearSignature OnMordathBeginDisappear;
};

/**
 * Manager object for the current game state (Boss fight specifically)
 */
UCLASS()
class MYTHOLOGICALBOSS_API AOverthroneGameState final : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Overthrone Game State | General")
		FOnGamePausedSignature OnGamePaused;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Overthrone Game State | General")
		FOnLightningStrikeSpawned OnLightningStrikeSpawned;

	// Trigger a boss stage
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State | Boss")
		void EnterBossStage(EBossStage_Mordath InBossStage) const;

	// Lock the boss
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State | Boss")
		void LockBoss() const;

	// Set mordath's health to zero and play the OnDeath event
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State | Boss")
		void KillMordath() const;

	// Spawns a ghost version of mordath
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State | Boss")
		void SpawnMordathGhost() const;

	// Restarts the current boss battle game
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void RestartGame();

	// Checks to see if the game is currently paused
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		bool IsGamePaused() const;

	// Pause the game and set input mode to UI
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void PauseGame() const;

	// UnPause the game and set input mode to Game
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void UnPauseGame() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void StartRecordingPlayerActions();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void StopRecordingPlayerActions();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game State")
		void ResetPlayerActions();

	void UpdatePlayerActionCount(const FString& ActionName, int32 Count);
	void UpdatePlayerActionCount(EAttackType_Player AttackType, int32 Count);
	void SavePlayerActions() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE APlayerCameraManager* GetCameraManager() const { return CameraManager; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE FVector GetBossLocation() const { return BossData.Location; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsBossDead() const { return BossData.bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsPlayerDead() const { return PlayerData.bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsPlayerAttacking() const { return PlayerData.CurrentAttackType != ATP_None; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State")
		FORCEINLINE bool IsBossAttacking() const { return BossData.bHasAttackBegun && (BossData.CurrentActionType == ATM_LongAttack_2 || BossData.CurrentActionType == ATM_LongAttack_3 ||
																						BossData.CurrentActionType == ATM_ShortAttack_1 || BossData.CurrentActionType == ATM_ShortAttack_2 || 
																						BossData.CurrentActionType == ATM_ShortAttack_3 || BossData.CurrentActionType == ATM_LongAttack_3 || 
																						BossData.CurrentActionType == ATM_SpecialAttack_1 || BossData.CurrentActionType == ATM_SpecialAttack_2 || 
																						BossData.CurrentActionType == ATM_SpecialAttack_3); }

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
		FORCEINLINE bool IsBossInFirstStage() const { return BossData.CurrentStage == Stage_1; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE bool IsBossInSecondStage() const { return BossData.CurrentStage == Stage_2; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE bool IsBossInThirdStage() const { return BossData.CurrentStage == Stage_3; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		int32 GetNumOfMordathGhostsAlive() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		bool IsBossTeleporting() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE EActionType_Mordath GetCurrentBossAttackType() const { return BossData.CurrentActionType; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Game State | Boss")
		FORCEINLINE FLinearColor GetBossDamageColor() const { return BossData.DamageColor; }

	// The player character
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Characters")
		class AYlva* Ylva;

	// The boss character
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Characters")
		class AMordath* Mordath;

	// The lock-on actor in the world
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game State | Lock-On")
		class ALockOn* LockOn;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Game State | Tutorial")
		uint8 bInTutorialSection : 1;

	// Character Data
	FPlayerData PlayerData;
	FBossData BossData;

	FVector LockOnLocation;

	UPROPERTY(BlueprintAssignable, Category = "Overthrone Game State | Characters")
		FOnMordathBaseDeathSignature OnMordathBaseDeath;

	class UCameraShake* CurrentCameraShake;

	class ABoundingBox* PlayArea{};

	TArray<ACharacter*> Mordaths;

protected:
	void BeginPlay() override;

private:
	APlayerController* PlayerController{};
	APlayerCameraManager* CameraManager{};

	uint8 bCanRecordPlayerData : 1;
};
