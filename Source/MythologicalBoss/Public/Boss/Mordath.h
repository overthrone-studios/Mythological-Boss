// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Combat/ComboData.h"
#include "Mordath.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Mordath : public FCharacterDebug
{
	GENERATED_BODY()

	// Log the distance between this and the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDistance : 1;

	// Log the direction from this to the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDirection : 1;

	// Log the delay time (in seconds) when a new combo is chosen to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogComboDelayTime : 1;

	// Log the current combo name to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCurrentCombo : 1;

	// Log the current stage combo to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCurrentStageCombo : 1;

	// Log the hit count to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogHits : 1;
};

USTRUCT(BlueprintType)
struct FBezier
{
	GENERATED_BODY()

	// The curve asset to use when in this state
	UPROPERTY(EditInstanceOnly)
		UCurveFloat* Curve;

	// Show debug information
	UPROPERTY(EditInstanceOnly)
		uint8 bDebug : 1;

	// How fast do we do this attack (1.0 = Normal, 2.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float PlaybackSpeed = 1.0f;

	// The height of the 2nd point of the bezier curve
	UPROPERTY(EditInstanceOnly)
		float CurveHeight = 0.0f;

	// The amount of units to offset the end point by
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float EndPointOffsetDistance = 0.0f;

	// Points on bezier
	FVector A, B, C;
};

USTRUCT(BlueprintType)
struct FMovementSettings_Mordath : public FMovementSettings
{
	GENERATED_BODY()

	// The movement speed of the boss when we're at mid range distance from the player
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 1000000.0f))
		float MidRangeWalkSpeed = 800.0f;
};

USTRUCT(BlueprintType)
struct FCameraShakes_Mordath
{
	GENERATED_BODY()

	// The camera shake to play when we are damaged
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Damaged;

	// The camera shake to play when we are damaged
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Stun;
};

USTRUCT(BlueprintType)
struct FComboSettings
{
	GENERATED_BODY()

	// Should the boss wait before initiating the next combo?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bDelayBetweenCombo : 1;

	// The time in seconds to delay before choosing a new combo
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float ComboDelayTime = 1.0f;

	// Adds a random range to ComboDelayTime
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float RandomDeviation = 0.1f;

	// Should the boss choose a random combo from the Combos list?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bChooseRandomCombo : 1;

	// A list of combos the boss character will choose from when in the first stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		TArray<UComboData*> FirstStageCombos;

	// A list of combos the boss character will choose from when in the second stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		TArray<UComboData*> SecondStageCombos;

	// A list of combos the boss character will choose from when in the third stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		TArray<UComboData*> ThirdStageCombos;
};

USTRUCT(BlueprintType)
struct FAttackSettings_Mordath : public FAttackSettings
{
	GENERATED_BODY()

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float SpecialAttackDamage = 250.0f;

	// The amount of time (in seconds) that the boss can be allowed to jump attack again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float JumpAttackCooldown = 2.0f;
};

USTRUCT(BlueprintType)
struct FDashSettings_Mordath
{
	GENERATED_BODY()

	// The amount of time (in seconds) that the boss can be allowed to dash attack again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float DashCooldown = 5.0f;
};

USTRUCT(BlueprintType)
struct FStunSettings_Mordath
{
	GENERATED_BODY()

	// The amount of time in seconds this boss should be stunned for
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.0f))
		float Duration = 0.8f;
};

USTRUCT(BlueprintType)
struct FCombatSettings_Mordath : public FCombatSettings
{
	GENERATED_BODY()

	// Settings that affect Mordath's attack values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "Attack")
		FAttackSettings_Mordath AttackSettings;

	// Settings that affect Mordath's dash values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "Dash")
		FDashSettings_Mordath DashSettings;

	// Settings that affect Mordath's stun values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "Stun")
		FStunSettings_Mordath StunSettings;

	// Maximum hits that can be taken before becoming invincible
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath", meta = (ClampMin = 0, ClampMax = 100))
		uint8 MaxHitsBeforeInvincibility = 3;

	// The amount of time (in seconds) that the boss can stay invincible after being damaged by the player
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath", meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float InvincibilityTimeAfterDamage = 1.5f;

	float RecentDamage = 0.0f;
};

UCLASS()
class MYTHOLOGICALBOSS_API AMordath final : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AMordath();

	// Returns the light attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FORCEINLINE float GetLightAttackDamage() const { return Combat.AttackSettings.LightAttackDamage; }

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FORCEINLINE float GetHeavyAttackDamage() const { return Combat.AttackSettings.HeavyAttackDamage; }

	// Returns the special attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FORCEINLINE float GetSpecialAttackDamage() const { return Combat.AttackSettings.SpecialAttackDamage; }

	// Returns the attack radius value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

	// Returns the recent damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FORCEINLINE float GetRecentDamage() const { return Combat.RecentDamage; }

	// Returns true if we are light attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsLightAttacking() const;

	// Returns true if we are heavy attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsHeavyAttacking() const;

	// Returns true if we are special attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsSpecialAttacking() const;

	// Returns true if we are in the first stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInFirstStage() const;

	// Returns true if we are in the second stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInSecondStage() const;

	// Returns true if we are in the third stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInThirdStage() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		float GetWalkSpeed() const;

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

	// List of debugging options
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		FDebug_Mordath Debug;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;
	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void OnLowHealth() override;

	void SendInfo();

	void FinishStun();

	UFUNCTION(BlueprintPure, Category = "Mordath | Misc")
		FRotator FacePlayer();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseComboWithDelay();
	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseCombo();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void NextAttack();

	void Die() override;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void DestroySelf();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseAttack();

	UFUNCTION(BlueprintPure, Category = "Mordath | Misc")
		float GetDistanceToPlayer() const;

	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FVector GetDirectionToPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void EnableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void DisableInvincibility();

	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsStunned();

	#pragma region Events
	// Called when the player's health is less than or equal to 0
	UFUNCTION()
		void OnPlayerDeath();

	UFUNCTION()
		void OnSecondStageHealth();

	UFUNCTION()
		void OnThirdStageHealth();

	UFUNCTION(BlueprintImplementableEvent, Category = "Mordath")
		void OnAfterTakeDamage();
	#pragma endregion 

	#pragma region Boss States
		#pragma region Idle
		UFUNCTION()
			void OnEnterIdleState();
		UFUNCTION()
			void UpdateIdleState();
		UFUNCTION()
			void OnExitIdleState();
		#pragma endregion 

		#pragma region Follow
		UFUNCTION()
			void OnEnterFollowState();
		UFUNCTION()
			void UpdateFollowState();
		UFUNCTION()
			void OnExitFollowState();
		#pragma endregion 

		#pragma region Think
		UFUNCTION()
			void OnEnterThinkState();
		UFUNCTION()
			void UpdateThinkState();
		UFUNCTION()
			void OnExitThinkState();
		#pragma endregion 

		#pragma region Light Attack 1
		UFUNCTION()
			void OnEnterLightAttack1State();
		UFUNCTION()
			void UpdateLightAttack1State();
		UFUNCTION()
			void OnExitLightAttack1State();
		#pragma endregion 

		#pragma region Light Attack 2
		UFUNCTION()
			void OnEnterLightAttack2State();
		UFUNCTION()
			void UpdateLightAttack2State();
		UFUNCTION()
			void OnExitLightAttack2State();
		#pragma endregion 

		#pragma region Light Attack 3
		UFUNCTION()
			void OnEnterLightAttack3State();
		UFUNCTION()
			void UpdateLightAttack3State();
		UFUNCTION()
			void OnExitLightAttack3State();
		#pragma endregion 

		#pragma region Heavy Attack 1
		UFUNCTION()
			void OnEnterHeavyAttack1State();
		UFUNCTION()
			void UpdateHeavyAttack1State();
		UFUNCTION()
			void OnExitHeavyAttack1State();
		#pragma endregion 

		#pragma region Heavy Attack 2
		UFUNCTION()
			void OnEnterHeavyAttack2State();
		UFUNCTION()
			void UpdateHeavyAttack2State();
		UFUNCTION()
			void OnExitHeavyAttack2State();
		#pragma endregion 

		#pragma region Heavy Attack 3
		UFUNCTION()
			void OnEnterHeavyAttack3State();
		UFUNCTION()
			void UpdateHeavyAttack3State();
		UFUNCTION()
			void OnExitHeavyAttack3State();
		#pragma endregion 

		#pragma region Damaged
		UFUNCTION()
			void OnEnterDamagedState();
		UFUNCTION()
			void UpdateDamagedState();
		UFUNCTION()
			void OnExitDamagedState();
		#pragma endregion 

		#pragma region Death
		UFUNCTION()
			void OnEnterDeathState();
		UFUNCTION()
			void UpdateDeathState();
		UFUNCTION()
			void OnExitDeathState();
		#pragma endregion 

		#pragma region Stunned
		UFUNCTION()
			void OnEnterStunnedState();
		UFUNCTION()
			void UpdateStunnedState();
		UFUNCTION()
			void OnExitStunnedState();
		#pragma endregion 

		#pragma region Laugh
		UFUNCTION()
			void OnEnterLaughState();
		UFUNCTION()
			void UpdateLaughState();
		UFUNCTION()
			void OnExitLaughState();
		#pragma endregion 

		#pragma region Dash
		UFUNCTION()
			void OnEnterDashState();
		UFUNCTION()
			void UpdateDashState();
		UFUNCTION()
			void OnExitDashState();
		#pragma endregion 

		#pragma region Beaten
		UFUNCTION()
			void OnEnterBeatenState();
		UFUNCTION()
			void UpdateBeatenState();
		UFUNCTION()
			void OnExitBeatenState();
		#pragma endregion 

		#pragma region Teleport
		UFUNCTION()
			void OnEnterTeleportState();
		UFUNCTION()
			void UpdateTeleportState();
		UFUNCTION()
			void OnExitTeleportState();
		#pragma endregion 
	#pragma endregion

	#pragma region Range States
		#pragma region Close
		UFUNCTION()
			void OnEnterCloseRange();
		UFUNCTION()
			void UpdateCloseRange();
		UFUNCTION()
			void OnExitCloseRange();
		#pragma endregion 

		#pragma region Mid
		UFUNCTION()
			void OnEnterMidRange();
		UFUNCTION()
			void UpdateMidRange();
		UFUNCTION()
			void OnExitMidRange();
		#pragma endregion 

		#pragma region Far
		UFUNCTION()
			void OnEnterFarRange();
		UFUNCTION()
			void UpdateFarRange();
		UFUNCTION()
			void OnExitFarRange();
		#pragma endregion 
	#pragma endregion

	#pragma region Boss Stages

	#pragma region Stage 1
	UFUNCTION()
		void OnEnterFirstStage();
	UFUNCTION()
		void UpdateFirstStage();
	UFUNCTION()
		void OnExitFirstStage();
	#pragma endregion 

	#pragma region Stage 2
	UFUNCTION()
		void OnEnterSecondStage();
	UFUNCTION()
		void UpdateSecondStage();
	UFUNCTION()
		void OnExitSecondStage();
	#pragma endregion 

	#pragma region Stage 3
	UFUNCTION()
		void OnEnterThirdStage();
	UFUNCTION()
		void UpdateThirdStage();
	UFUNCTION()
		void OnExitThirdStage();
	#pragma endregion 
	#pragma endregion 

	#pragma region Components
	// The boss's range Finite State Machine
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Mordath")
		class UFSM* RangeFSM;

	// The boss's stage Finite State Machine
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Mordath")
		class UFSM* StageFSM;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Mordath")
		class UTeleportationComponent* TeleportationComponent;
	#pragma endregion

	// The radius in which the boss character will accept that it has arrived to the player's location
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 1.0f, ClampMax = 100000.0f), DisplayName = "Close Range Radius")
		float AcceptanceRadius = 200.0f;

	// The radius of the mid range area
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 1.0f, ClampMax = 100000.0f))
		float MidRangeRadius = 400.0f;

	// How long (in seconds) should the boss stay dead before being destroyed?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 0.01f, ClampMax = 100000.0f))
		float DeathTime = 2.0f;

	// The distance to ray cast from the boss's location (for destructible actor detection)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath", meta = (ClampMin = 1.0f, ClampMax = 1000.0f))
		float BoxDetectionDistance = 130.0f;

	// The health value where we enter the second stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float SecondStageHealth = 0.6f;

	// The health value where we enter the third stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float ThirdStageHealth = 0.3f;

	// Mordath's movement settings
	UPROPERTY(EditInstanceOnly, Category = "Mordath", DisplayName = "Movement")
		FMovementSettings_Mordath MovementSettings;

	// The combo settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Combat")
		FComboSettings ComboSettings;

	// Properties of the boss's combat settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Combat")
		FCombatSettings_Mordath Combat;

	// List of camera shakes
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakes_Mordath CameraShakes;

	int8 ComboIndex = 0; // This is used to choose a random index in the combos list

	// Our custom AI controller
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | AI")
		class ABossAIController* BossAIController{};
	
	// Cached anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Animation")
		class UMordathAnimInstance* MordathAnimInstance{};

	// The combo we are using
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		UComboData* ChosenCombo;

	// Used to iterate, select or remove a combo, this to avoid touching the actual combos list
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		TArray<UComboData*> CachedCombos;

private:
	FTimerHandle UpdateInfoTimerHandle;

	FTimerHandle StunExpiryTimerHandle;

	FTimerHandle JumpAttackCooldownTimerHandle;
	FTimerHandle DashCooldownTimerHandle;

	FTimerHandle ComboDelayTimerHandle;
	FTimerHandle InvincibilityTimerHandle;

	// Access to player information
	AOverthroneCharacter* PlayerCharacter;
};
