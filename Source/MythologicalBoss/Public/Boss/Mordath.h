// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "Combat/ComboData.h"
#include "Mordath.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Mordath
{
	GENERATED_BODY()

	// Log the distance between this and the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDistance : 1;

	// Log the direction from this to the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDirection : 1;

	// Log the new chosen combo to the viewport or console
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogChosenCombo : 1;

	// Log the delay time (in seconds) when a new combo is chosen to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogComboDelayTime : 1;

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
struct FCameraShake_Mordath
{
	GENERATED_BODY()
		
	// The camera shake to play
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TSubclassOf<class UCameraShake> Shake;

	// Scale the shake intensity based on the amount of hits we have recieved
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bScaleIntensityBasedOnHits : 1;

	// The intensity of the camera shake
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f, ClampMax = 1000.0f, EditCondition = "!bScaleIntensityBasedOnHits"))
		float Intensity = 1.0f;
};

USTRUCT(BlueprintType)
struct FCameraShakes_Mordath
{
	GENERATED_BODY()

	// The camera shake to play when we are damaged
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShake_Mordath Damaged;

	// The camera shake to play when we are damaged
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShake_Mordath Stun;
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

	// A list of combos the boss character will choose from
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.0f))
		TArray<UComboData*> Combos;
};

USTRUCT(BlueprintType)
struct FAttackSettings_Mordath
{
	GENERATED_BODY()

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float SpecialAttackDamage = 250.0f;

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float AttackDistance = 250.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 1.0f, ClampMax = 1000.0f))
		float AttackRadius = 50.0f;

	// Properties of the jump attack curve
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "Jump Attack Curve")
		FBezier JumpAttack_Bezier;

	// The amount of time (in seconds) that the boss can be allowed to jump attack again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float JumpAttackCooldown = 2.0f;
};

USTRUCT(BlueprintType)
struct FDashSettings_Mordath
{
	GENERATED_BODY()

	// Properties of the dash curve
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "Dash Curve")
		FBezier Dash_Bezier;

	// The amount of time (in seconds) that the boss can be allowed to dash attack again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float DashCooldown = 5.0f;
	
	// The distance of how far we can dash in a given direction
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 100000.0f))
		float DashDistance = 1000.0f;
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
struct FCombatSettings_Mordath
{
	GENERATED_BODY()

	// Should we stop animations when we are hit?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bEnableHitStop : 1;

	// The amount of time (in seconds) we stay paused when we hit something
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.001f, ClampMax = 10.0f, EditCondition = "bEnableHitStop"))
		float HitStopTime = 0.1f;

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
};

UCLASS()
class MYTHOLOGICALBOSS_API AMordath final : public ACharacter
{
	GENERATED_BODY()

public:
	AMordath();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		class UFSM* GetFSM() const { return FSM; }

	// Returns the light attack damage value
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetLightAttackDamage() const { return Combat.AttackSettings.LightAttackDamage; }

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetHeavyAttackDamage() const { return Combat.AttackSettings.HeavyAttackDamage; }

	// Returns the special attack damage value
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetSpecialAttackDamage() const { return Combat.AttackSettings.SpecialAttackDamage; }

	// Returns the attack distance value
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetAttackRange() const { return Combat.AttackSettings.AttackDistance; }

	// Returns the attack radius value
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SendInfo();
	bool ShouldDestroyDestructibleObjects();

	void FinishStun();
	void FinishCooldown();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FRotator FacePlayer();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void ChooseComboWithDelay();
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void ChooseCombo();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void NextAttack();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void Die();
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void DestroySelf();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void PauseAnims();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void UnPauseAnims();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void ChooseAttack();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		float GetDistanceToPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FVector GetDirectionToPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void EnableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void DisableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		bool IsInvincible();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void BeginJumpAttack();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void BeginDash(enum EDashType_Combo DashType);
	
	UFUNCTION()
		void DoDash();
	
	UFUNCTION()
		void DoJumpAttack();

	UFUNCTION()
		void FinishJumpAttack();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		bool IsStunned();

	#pragma region Events
	// Called when the player's health is less than or equal to 0
	UFUNCTION()
		void OnPlayerDeath();

	// Called when the dash timeline component has finished
	UFUNCTION()
		void OnDashFinished();
	#pragma endregion 

	// Boss states
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

	// Boss ranges
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

	// The boss's Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UFSM* FSM;

	// The boss's range Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UFSM* RangeFSM;

	// Used for calculating the jump curve
	UPROPERTY()
		class UTimelineComponent* JumpAttackTimelineComponent;

	// Used for calculating the dash curve
	UPROPERTY()
		class UTimelineComponent* DashTimelineComponent;

	// The starting health of the boss
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 100.0f, ClampMax = 100000.0f))
		float StartingHealth = 1000.0f;

	// The current health of the boss
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 100.0f, ClampMax = 100000.0f))
		float Health = 1000.0f;

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

	// List of debugging options
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		FDebug_Mordath Debug;

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float WalkSpeed = 600.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float RunSpeed = 1000.0f;

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

	uint8 HitCounter = 0; // To keep track of how many hits we've taken

	// True when we have been damaged
	uint8 bIsHit : 1;

	// Cached world pointer
	UWorld* World{};

	// Our custom AI controller
	class ABossAIController* BossAIController{};
	
	// The skeletal mesh representing the player
	USkeletalMesh* SkeletalMesh;

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached anim instance, to control and trigger animations
	class UMordathAnimInstance* AnimInstance{};

	// Cached Overthrone HUD reference, used to access HUDs
	class AOverthroneHUD* OverthroneHUD{};

	// Cached main HUD
	class UMainPlayerHUD* MainHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	// The combo we are using
	UComboData* ChosenCombo;

	// Used to iterate, select or remove a combo, this to avoid touching the actual combos list
	TArray<UComboData*> CachedCombos;

private:
	void InitTimelineComponent(class UTimelineComponent* InTimelineComponent, class UCurveFloat* InCurveFloat, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName);

	FBezier& JumpAttack_Bezier = Combat.AttackSettings.JumpAttack_Bezier;
	FBezier& Dash_Bezier = Combat.DashSettings.Dash_Bezier;

	FTimerHandle UpdateInfoTimerHandle;

	FTimerHandle StunExpiryTimerHandle;
	FTimerHandle DeathExpiryTimerHandle;

	FTimerHandle JumpAttackCooldownTimerHandle;
	FTimerHandle DashCooldownTimerHandle;

	FTimerHandle ComboDelayTimerHandle;
	FTimerHandle InvincibilityTimerHandle;
	FTimerHandle HitStopTimerHandle;

	// Access to player information
	ACharacter* PlayerCharacter;
	APlayerController* PlayerController;
};
