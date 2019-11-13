// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Queue.h"
#include "Components/AttackComboComponent.h"
#include "OverthroneStructs.h"
#include "Ylva.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleInputSignature, FKey, Key);

USTRUCT(BlueprintType)
struct FDebug_Ylva : public FCharacterDebug
{
	GENERATED_BODY()

	// Log the hit component name to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogComponentHits : 1;

	// Log the attack queue information to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogAttackQueue : 1;
	
	// Log the dash queue information to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDashQueue : 1;
};

USTRUCT(BlueprintType)
struct FMovementSettings_Ylva : public FMovementSettings
{
	GENERATED_BODY()

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float RunSpeed = 700.0f;

	// The maximum movement speed while locking on to the boss
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float LockOnWalkSpeed = 400.0f;

	// The lean roll weight
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float LeanRollOffset = 10.0f;

	// The lean pitch weight
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float LeanPitchOffset = 0.7f;

	// The strength of the knockback effect
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float KnockbackForce = 500.0f;

	// Should the player stop moving when attacking?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bStopMovingWhenAttacking : 1;

	// Should the player stop moving when taking damage?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bStopMovingWhenDamaged : 1;
};

USTRUCT(BlueprintType)
struct FAttackQueue_Ylva
{
	GENERATED_BODY()

	// The point in time of the light attack animation where the attack queueing system would queue the next attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float LightAttackTriggerTime = 0.3f;

	// The point in time of the heavy attack animation where the attack queueing system would queue the next attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float HeavyAttackTriggerTime = 0.2f;

	// The amount of time (in seconds) the attack queue will clear itself. If the queue takes longer than the specified time (in seconds), clear the queue.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float ExpiryTime = 0.8f;
};

USTRUCT(BlueprintType)
struct FAttackSettings_Ylva : public FAttackSettings
{
	GENERATED_BODY()

	// The attack damage we deal when charge attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float ChargeAttackDamage = 200.0f;

	// The amount of time (in seconds) the attack queue will clear itself. If the queue takes longer than the specified time (in seconds), clear the queue.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float CloseRangeAttackRotationSpeed = 5.0f;

	// The rotation speed the character will rotate at to face the boss when in the super close range
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float SuperCloseRangeAttackRotationSpeed = 2.0f;
};


USTRUCT(BlueprintType)
struct FDashAttackSettings_Ylva
{
	GENERATED_BODY()

	// The camera animation to play when dash has been performed perfectly
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UCameraAnim* CameraAnim;

	class UCameraAnimInst* CameraAnimInst;

	// The attack damage we deal when performing a dash attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float DashAttackDamage = 300.0f;

	// The value to set when we've successfully dashed at the right time when the boss is attacking (0.0 = Time stands still, 0.5 = Half time speed, 1 = Normal time speed)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float TimeDilationOnPerfectDash = 0.2f;

	// The value to set when dash attacking (0.0 = Time stands still, 0.5 = Half time speed, 1 = Normal time speed)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float TimeDilationWhileAttacking = 0.6f;

	// The camera pitch to set when starting to dash attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 360.0f))
		float CameraPitchOnSuccess = 350.0f;
};

USTRUCT(BlueprintType)
struct FChargeSettings_Ylva
{
	GENERATED_BODY()

	// The amount of frames to wait after pressing the 'charge attack' key before intiating the charge attack
	UPROPERTY(EditInstanceOnly, DisplayName = "Charge Intial Delay Frames")
		uint8 MaxChargeKeyHeldFrames = 10;

	// The camera animation to play when parry has succeeded
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UCameraAnim* ChargeCameraAnim;

	class UCameraAnimInst* ChargeCameraAnimInst;

	// The force effect to play while charging
	UPROPERTY(EditInstanceOnly)
		class UForceFeedbackEffect* ChargeAttackForce;

	// The force effect to play when we've landed a charge attack hit
	UPROPERTY(EditInstanceOnly)
		class UForceFeedbackEffect* ChargeAttackEndForce;
};

USTRUCT(BlueprintType)
struct FDefenseSettings_Ylva
{
	GENERATED_BODY()

	// This value will be used to buffer the damage received when the boss hits the player while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float DamageBuffer = 0.5f;

	// The block idle anim montage to play
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* BlockIdle;

	// The sound to play when we've blocked an attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UHitSoundData* ShieldHitSoundData;
};

USTRUCT(BlueprintType)
struct FParrySettings_Ylva
{
	GENERATED_BODY()
	
	// The parry anim montage to play
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* ParryMontage;

	// The sound to play when we've parried an attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UHitSoundData* ParryHitSoundData;

	// The parry window time in frames
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0))
		uint8 MinParryFrame = 5;

	// The parry window time in frames
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0))
		uint8 MaxParryFrame = 20;

	// This value will be used when a parry is successful (slowing down time)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float TimeDilationOnSuccessfulParry = 0.4f;

	// The camera rotation pitch to set when parry has succeeded
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 270.0f, ClampMax = 360.0f))
		float CameraPitchOnSuccess = 360.0f;
};

USTRUCT(BlueprintType)
struct FCombatSettings_Ylva : public FCombatSettings
{
	GENERATED_BODY()

	// Settings that affect Ylva's attack values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FAttackSettings_Ylva AttackSettings;

	// Settings that affect the attack queue
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FAttackQueue_Ylva AttackQueue;

	// Settings that affect Ylva's attack values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FDashAttackSettings_Ylva DashAttackSettings;

	// Settings that affect Ylva's charge settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FChargeSettings_Ylva ChargeSettings;

	// Settings that affect blocking values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FDefenseSettings_Ylva BlockSettings;

	// Settings that affect parry values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FParrySettings_Ylva ParrySettings;

	// The force effect to play when a light attack has been landed on a blocking hit
	UPROPERTY(EditInstanceOnly)
		class UForceFeedbackEffect* LightAttackForce;
	
	// The force effect to play when a heavy attack has been landed on a blocking hit
	UPROPERTY(EditInstanceOnly)
		class UForceFeedbackEffect* HeavyAttackForce;

	// The force effect to play when being damage by an electric shield
	UPROPERTY(EditInstanceOnly)
		class UForceFeedbackEffect* ElectricShieldForce;
};

/*
 * The player character we control
 */
UCLASS()
class MYTHOLOGICALBOSS_API AYlva final : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AYlva();

	void AddDebugMessages() override;

	void ApplyHitStop() override;

	// Returns CameraBoom component
	UFUNCTION(BlueprintCallable, Category = "Ylva | Components")
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera component
	UFUNCTION(BlueprintCallable, Category = "Ylva | Components")
		FORCEINLINE class UPlayerCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void StopMovement() override;

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void ResumeMovement();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void SpawnGhost();

	// Returns true if we have taken 1 or more hits
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE bool HasTakenAnyDamage() const { return HitCounter_Persistent > 0; }

	// Returns the light attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		float GetLightAttackDamage() const;

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		float GetHeavyAttackDamage() const;

	// Returns the charge attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		float GetChargeAttackDamage() const;

	// Returns the dash attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		float GetDashAttackDamage() const;

	// Returns the attack radius value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

	// Returns true if we are light attacking
	UFUNCTION(BlueprintCallable, Category = "Ylva | Combat")
		bool IsLightAttacking() const;

	// Returns true if we are heavy attacking
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsHeavyAttacking() const;

	// Returns true if we are doing any attack
	bool IsAttacking() const override;

	// Returns true if we are charge attacking
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsChargeAttacking() const;

	// Did we successfully parry?
	UFUNCTION(BlueprintCallable, Category = "Ylva | Combat")
		bool IsParrySuccessful() const;

	// Returns true if we are currently dashing
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsDashing() const;

	// Returns true if we are performing a dash attack
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsDashAttacking() const;

	// Returns true if our forward input is > 0.0f AND our right input == 0.0f
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsMovingForward() const;

	// Returns true if our forward input is < 0.0f AND our right input == 0.0f
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsMovingBackward() const;

	// Returns true if our right input is > 0.0f AND our forward input == 0.0f
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsMovingRight() const;

	// Returns true if our right input is < 0.0f AND our forward input == 0.0f
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsMovingLeft() const;

	// Returns true if our forward input or right input != 0.0f
	bool IsMovingInAnyDirection() const override;

	// Returns true if we are low on stamina
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsLowStamina() const;

	// Returns true if we are currently locked on to the boss
	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		bool IsLockedOn() const;

	// Returns true if we are currently idling
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsIdle() const;

	// Returns true if we are currently walking
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsWalking() const;

	// Returns true if we are currently blocking
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsBlocking() const;

	// Returns true if we are currently running
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsRunning() const;

	// Returns true if we are damaged by an attack
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsDamaged() const;

	// Returns true if we are currently parrying
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsParrying() const;

	// Returns true if we are allowed to perform a dash attack
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool CanDashAttack() const;

	// Returns true if we have triggered a perfect dash
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsPerfectDashing() const;

	// Returns true if we are being pushed back
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsBeingPushedBack() const;

	// Returns true if we are allowed to lock on
	UFUNCTION(BlueprintPure, Category = "Ylva | Lock-On Camera")
		bool CanLockOn() const;

	// Increases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva | Charge Attack")
		void IncreaseCharge();

	// Give the maximum charge
	UFUNCTION(BlueprintCallable, Category = "Ylva | Charge Attack")
		void GiveMaxCharge();

	UFUNCTION(BlueprintPure, Category = "Ylva | Feedback")
		class UForceFeedbackEffect* GetCurrentForceFeedback() const;

	void OnAttackLanded(const FHitResult& HitResult) override;

	bool IsLocked() const override;

	// Turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
		float TurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
		float LookUpRate;

	// Ylva's debug options
	UPROPERTY(EditInstanceOnly, Category = "Ylva Debug")
		FDebug_Ylva Debug;

	UPROPERTY(BlueprintAssignable, Category = "Input")
		FOnHandleInputSignature OnHandleInput;

	UPROPERTY(BlueprintReadWrite, Category = "Ylva")
		uint8 bCanPausedGame : 1;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side/strafing input
	void MoveRight(float Value);

	// Called to turn the camera
	void AddControllerYawInput(float Val) override;

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void HardLockOnTo(const FVector& TargetLocation, float DeltaTime, bool bControlPitch = true);
	void SoftLockOnTo(const FVector& TargetLocation, float DeltaTime);

	void FaceBoss(float DeltaTime, float RotationSpeed = 10.0f);
	void FaceRotation_Custom(FRotator NewControlRotation, float DeltaTime, float RotationSpeed);
	void FaceBoss_Instant();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;

	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void BroadcastExitLowHealth() override;
	void BroadcastLowStamina();
	void BroadcastExitLowStamina();

	void Die() override;

	void ToggleLockSelf() override;
	void UnlockSelf() override;
	void LockSelf() override;

	float CalculateDirection() const;

	void CalculateRollLean(float DeltaTime);
	void CalculatePitchLean(float DeltaTime);

	class UYlvaDifficultyData* GetDifficultyData() const;

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void PauseAllAudioSources();
	
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResumeAllAudioSources();

	UFUNCTION()
		void HandleInput(FName ActionName);

	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		float GetDistanceToBoss() const;

	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		float GetNearestDistanceToBoss() const;

	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		FVector GetDirectionToBoss() const;
	
	#pragma region Combat
	// Called via input to enter the light attacking state
	void LightAttack();

	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void BeginLightAttack(class UAnimMontage* AttackMontage);

	// Called via input to enter the heavy attacking state
	void HeavyAttack();

	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void BeginHeavyAttack(class UAnimMontage* AttackMontage);

	void Attack_Queued();

	void ClearAttackQueue();

	// Called via input to begin the charge attack event
	void StartChargeAttack();

	// Called via input when releasing the charge attack key
	void ReleaseChargeAttack();

	// Called 0.2 seconds after ReleaseChargeAttack()
	void FinishChargeAttack();

	UFUNCTION()
		void OnParryBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called via input to enter the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void Block();

	// Called via input to exit the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void StopBlocking();

	void BeginTakeDamage(float DamageAmount, const FDamageEvent& DamageEvent) override;

	void ApplyDamage(float DamageAmount, const FDamageEvent& DamageEvent) override;

	void EndTakeDamage(const FDamageEvent& DamageEvent) override;
	#pragma endregion

	#pragma region Movement
	// Called via input to enter the running state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Movement")
		void Run();

	UFUNCTION(BlueprintCallable,Category = "Ylva | Movement")
		void UpdateIsRunHeld();

	// Called via input to exit the running state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Movement")
		void StopRunning();

	// Called via input to start dashing
	UFUNCTION(BlueprintCallable,Category = "Ylva | Movement")
		void Dash();

	// Set a timer to start the dash cooldown
	UFUNCTION(BlueprintCallable,Category = "Ylva | Movement")
		void StartDashCooldown();

	void Dash_Queued();
	#pragma endregion

	#pragma region LockOn
	UFUNCTION(BlueprintCallable, Category = "Ylva | Lock-On Camera")
		void ToggleLockOn();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Lock-On Camera")
		void EnableLockOn();

	UFUNCTION()
		void OnLockOnEnabled();

	UFUNCTION()
		void OnLockOnDisabled();
	#pragma endregion 

	#pragma region Controls
	// Called via input to pause the game
	UFUNCTION(BlueprintCallable,Category = "Ylva | Controls")
		void Pause();
	#pragma endregion

	#if !UE_BUILD_SHIPPING
	#pragma region Debug
	// Kill the player
	void Debug_Die();

	// Reset the player's achievements
	void Debug_ResetFeats();

	// Give the player full health
	void Debug_MaxHealth();

	// Give the player full stamina
	void Debug_RefillStamina();

	// Give the player full charge
	void Debug_MaxCharge();

	// Buff overall player stats
	void Debug_ToggleBuff();

	// Enable/Disable the boss's locked state
	void Debug_ToggleLockBoss();

	// Toggle animation blend out on low stamina
	void Debug_ToggleLowStaminaAnimBlendOut();

	// Called via input to trigger boss stage 1 behaviour
	void Debug_BossStage1();
	
	// Called via input to trigger boss stage 2 behaviour
	void Debug_BossStage2();
	
	// Called via input to trigger boss stage 3 behaviour
	void Debug_BossStage3();

	// Called via input to trigger boss death
	void Debug_KillBoss();

	// Called via input to trigger mordath's ghost to spawn in the world
	void Debug_SpawnMordathGhost();

	// Called via input to show the FSM Visualizer widget
	void ShowPlayerFSMVisualizer();

	// Called via input to show the Boss FSM Visualizer widget
	void ShowBossFSMVisualizer();

	// Called via input to show the FSM Visualizer widget
	void ShowMainHUD();

	// Called via input to show the FSM Visualizer widget
	void ShowNoHUD();
	#pragma endregion
	#endif

	#pragma region Stamina
	// Called every frame
	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void RegenerateStamina(float Rate);

	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void UpdateStamina(float StaminaToSubtract);

	// Add stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void IncreaseStamina(float Amount);

	// Subtract stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void DecreaseStamina(float Amount);

	// Set a new stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void SetStamina(float NewStaminaAmount);

	// Resets stamina back to default
	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void ResetStamina();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Stamina")
		void StartLosingStamina();

	UFUNCTION()
		void LoseStamina();

	UFUNCTION()
		void FinishLosingStamina();
	#pragma endregion 

	#pragma region Charge Attack
	// Decreases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DecreaseCharge();

	// Decreases the charge meter
	void DecreaseCharge(float Amount);

	// Resets the charge meter to 0
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetCharge();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Charge Attack")
		void StartGainingCharge(float Amount);

	UFUNCTION()
		void GainCharge();

	UFUNCTION()
		void FinishGainingCharge();
	#pragma endregion 

	#pragma region God Mode
	UFUNCTION(BlueprintCallable, Category = "Ylva | Misc")
		void EnableGodMode();
	
	UFUNCTION(BlueprintCallable, Category = "Ylva | Misc")
		void DisableGodMode();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Misc")
		void ToggleGodMode();
	#pragma endregion

	#pragma region Feats
	UFUNCTION(BlueprintCallable, Category = "Ylva | Feats")
		void OnUntouchableFeatAchieved();
	#pragma endregion 

	#pragma region Events
	UFUNCTION()
		void OnGamePaused(bool bIsPaused);

	// Called when our health is below the given threshold
	void OnLowHealth() override;

	// Called when exiting the low health state
	void OnExitLowHealth() override;

	// Called when the boss's health is less than or equal to zero
	UFUNCTION(BlueprintImplementableEvent)
		void OnBossDeath();

	UFUNCTION()
		void OnBossDeath_Implementation();

	// Called when our stamina value is below the given threshold
	UFUNCTION()
		void OnLowStamina();

	// Called when we were on low stamina
	UFUNCTION()
		void OnExitLowStamina();

	// Called when our combo tree has been reset
	UFUNCTION(BlueprintImplementableEvent)
		void OnComboReset();

	UFUNCTION()
		void OnComboReset_Implementation();

	UFUNCTION()
		void OnComboMultiplierReached();

	// Called when our anim montage attack animations have finished
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnAttackEnd_Implementation(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnChargeMeterFull();

	// Called when we are light attacking
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnBeginLightAttack();
	
	// Called when we are heavy attacking
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnBeginHeavyAttack();

	// Called when we have successfully completed a light attack combo
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnLightAttackComboCompleted();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAfterTakeDamage();

	UFUNCTION()
		void OnEnterMordathEnergySphere();

	UFUNCTION()
		void OnExitMordathEnergySphere();

	UFUNCTION()
		void OnMordathDisappeared();

	UFUNCTION()
		void OnMordathReappeared();

	UFUNCTION()
		void OnMordathBaseDeath();
	#pragma endregion

	#pragma region Any States
	UFUNCTION()
		void OnEnterAnyState(int32 ID, FName Name);
	UFUNCTION()
		void UpdateAnyState(int32 ID, FName Name, float Uptime, int32 Frames);
	UFUNCTION()
		void OnExitAnyState(int32 ID, FName Name);
	#pragma endregion 

	#pragma region Player States
		#pragma region Idle
		UFUNCTION()
			void OnEnterIdleState();
		UFUNCTION()
			void UpdateIdleState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitIdleState();
		#pragma endregion 

		#pragma region Walk
		UFUNCTION()
			void OnEnterWalkState();
		UFUNCTION()
			void UpdateWalkState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitWalkState();
		#pragma endregion 

		#pragma region Run
		UFUNCTION()
			void OnEnterRunState();
		UFUNCTION()
			void UpdateRunState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitRunState();
		#pragma endregion 

		#pragma region Shield Block
		UFUNCTION()
			void OnEnterBlockingState();
		UFUNCTION()
			void UpdateBlockingState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitBlockingState();
		#pragma endregion 

		#pragma region Damaged
		UFUNCTION()
			void OnEnterDamagedState();
		UFUNCTION()
			void UpdateDamagedState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDamagedState();
		#pragma endregion 

		#pragma region Death
			void OnEnterDeathState() override;
			void UpdateDeathState(float Uptime, int32 Frames) override;
			void OnExitDeathState() override;
		#pragma endregion 

		#pragma region Charge Attack
		UFUNCTION()
			void OnEnterChargeAttackState();
		UFUNCTION()
			void UpdateChargeAttackState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitChargeAttackState();
		#pragma endregion 

		#pragma region Charge Attack
		UFUNCTION()
			void OnEnterPushBackState();
		UFUNCTION()
			void UpdatePushBackState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitPushBackState();
		#pragma endregion 

		#pragma region Shocked
		UFUNCTION()
			void OnEnterShockedState();
		UFUNCTION()
			void UpdateShockedState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitShockedState();
		#pragma endregion 

		#pragma region Attack
		UFUNCTION()
			void OnEnterAttackState();
		UFUNCTION()
			void UpdateAttackState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitAttackState();
		#pragma endregion 

		#pragma region Dash Attack
		UFUNCTION()
			void OnEnterDashAttackState();
		UFUNCTION()
			void UpdateDashAttackState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDashAttackState();
		#pragma endregion 

		#pragma region Shield Hit
		UFUNCTION()
			void OnEnterShieldHitState();
		UFUNCTION()
			void UpdateShieldHitState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitShieldHitState();
		#pragma endregion 

		#pragma region Dash
		UFUNCTION()
			void OnEnterDashState();
		UFUNCTION()
			void UpdateDashState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDashState();
		#pragma endregion 

		#pragma region Parry
		UFUNCTION()
			void OnEnterParryState();
		UFUNCTION()
			void UpdateParryState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitParryState();
		#pragma endregion 

		#pragma region Locked
			void OnEnterLockedState() override;
			void UpdateLockedState(float Uptime, int32 Frames) override;
			void OnExitLockedState() override;
		#pragma endregion 
	#pragma endregion

	#pragma region Misc
	// Resets global time dilation to 1
	UFUNCTION(BlueprintCallable,Category = "Ylva | Misc")
		void ResetGlobalTimeDilation();

	// Called when the player is killed (Health <= 0)
	UFUNCTION(BlueprintCallable,Category = "Ylva | Misc")
		void Respawn();

	// Returns the sword static mesh components attached to the left hand bone
	UFUNCTION(BlueprintCallable,Category = "Ylva | Misc")
		UStaticMeshComponent* GetLeftHandSword() const;

	// Returns the sword static mesh components attached to the right hand bone
	UFUNCTION(BlueprintCallable,Category = "Ylva | Misc")
		UStaticMeshComponent* GetRightHandSword() const;
	#pragma endregion

	#pragma region Components
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// The camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UPlayerCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UChargeAttackComponent* ChargeAttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UAttackComboComponent* AttackComboComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDashComponent* DashComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* ParryCollisionComponent;

	// This timeline plays when we are regenerating stamina
	FTimeline StaminaRegenTimeline;

	// This timeline plays when we are building charge
	FTimeline ChargeAttackTimeline;
	#pragma endregion

	// The float curve to use when regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina")
		class UCurveFloat* StaminaRegenCurve;

	// The float curve to use when building charge
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Charge Attack")
		class UCurveFloat* ChargeAttackCurve;

	// Ylva's movement settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", DisplayName = "Movement")
		FMovementSettings_Ylva MovementSettings;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		class UYlvaDifficultyData* CasualDifficultyData;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		class UYlvaDifficultyData* ExperiencedDifficultyData;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		class UYlvaDifficultyData* RealisticDifficultyData;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		UParticleSystem* SlashParticle;

	// The amount of time (in seconds) we are electrocuted for when we are hit by a lightning strike
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat", DisplayName = "Electric Shock Time")
		float ShockTime = 2.0f;

	// Ylva's combat settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		FCombatSettings_Ylva Combat;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", DisplayName = "Ghost Blueprint Class")
		TSubclassOf<AActor> GhostClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health")
		USoundBase* LowHealthSound;

	// Cached player's anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Ylva | Animation")
		class UYlvaAnimInstance* YlvaAnimInstance{};

	UPROPERTY(BlueprintReadOnly)
		float FastestTime = 0.0f;

private:
	TQueue<EAttackType_Player> AttackQueue;
	TQueue<uint8> DashQueue;

	class UYlvaDifficultyData* CurrentDifficultyData;

	float YawInput = 0.0f;

	float LockedForwardInput = 0.0f, LockedRightInput = 0.0f;

	FVector StartAttackLocation;
	FVector EndAttackLocation;

	FVector CurrentLockOnLocation;

	FVector DirectionToBoss;
	float DistanceToBoss;

	float PlayerLeanRollAmount = 0.0f;
	float PlayerLeanPitchAmount = 0.0f;

	float OriginalAttackRadius = 20.0f;

	uint8 MaxDashInQueue = 1;

	uint8 bChargeKeyPressed : 1;
	uint8 ChargeKeyHeldFrames = 0;

	uint8 bWantsDash : 1;

	uint8 bWasRunning : 1;

	uint8 bGodMode : 1;
	uint8 bCanDash : 1;

	uint8 bIsParryBoxHit : 1;

	uint8 bHasBeenDamaged : 1;

	uint8 bCanRun : 1;
	uint8 bPerformingArtificialMovement : 1;

	uint8 bPerfectlyTimedDash : 1;

	uint8 bIsRunKeyHeld : 1;
	uint8 bIsBuffed : 1;

	uint8 bWasLockedOn : 1;

	uint8 bWasLowStaminaEventTriggered : 1;

	uint8 HitCounter_Persistent = 0;

	float ChargeAttackHoldFrames = 0;

	FTimerHandle TH_ChargeAttackRelease;
	FTimerHandle TH_ChargeLoss;
	
	FTimerHandle TH_AttackQueue;
	FTimerHandle TH_AttackQueueExpiry;

	FTimerHandle TH_DashQueue;

	FPlayerAttack_Data* CurrentAttack_Data;

	class USwordComponent* LSword;
	class USwordComponent* RSword;

	class UForceFeedbackEffect* CurrentForceFeedback;

	class APlayerCameraManager* CameraManager;

	class UAudioComponent* LowHealthAudioComponent;

	class UFeatData* UntouchableFeat;
};
