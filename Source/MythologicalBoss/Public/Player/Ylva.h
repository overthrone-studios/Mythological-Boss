// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Queue.h"
#include "Ylva.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Ylva : public FCharacterDebug
{
	GENERATED_BODY()

	// Draw the teleport circle around the player
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bShowTeleportRadius : 1;

	// Log the hit component name to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogComponentHits : 1;
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

	// Should the player stop moving when attacking?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bStopMovingWhenAttacking : 1;

	// Should the player stop moving when taking damage?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bStopMovingWhenDamaged : 1;
};

USTRUCT(BlueprintType)
struct FLockOnSettings
{
	GENERATED_BODY()

	// Should the camera focus on the boss?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 10.0f, ClampMax = 1000.0f))
		bool bShouldLockOnTarget = false;

	// The target pitch when locking on
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 270.0f, ClampMax = 360.0f))
		float LockOnPitch = 350.0f;
};

USTRUCT(BlueprintType)
struct FAttackSettings_Ylva : public FAttackSettings
{
	GENERATED_BODY()

	// The attack damage we deal when charge attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float ChargeAttackDamage = 200.0f;

	// The point in time of the heavy attack animation where the attack queueing system would queue the next attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float LightAttackQueueTriggerTime = 0.3f;

	// The point in time of the light attack animation where the attack queueing system would queue the next attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float HeavyAttackQueueTriggerTime = 0.6f;

	// The amount of time (in seconds) the attack queue will clear itself. If the queue takes longer than the specified time (in seconds), clear the queue.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.01f))
		float AttackQueueExpiryTime = 0.5f;
};

USTRUCT(BlueprintType)
struct FChargeSettings_Ylva
{
	GENERATED_BODY()

	// The camera animation to play when parry has succeeded
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UCameraAnim* ChargeCameraAnim;

	class UCameraAnimInst* ChargeCameraAnimInst;
};

USTRUCT(BlueprintType)
struct FCameraShakes_Ylva : public FCameraShakes
{
	GENERATED_BODY()

	// The camera shake to play when we are blocking the boss's attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData ShieldHit;

	// The camera shake to play while idleling
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Idle;

	// The camera shake to play while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Walk;

	// The camera shake to play while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Run;

	// The camera shake to play while charging our attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Charge;

	// The camera shake to play when we release our charge attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData ChargeEnd;
};

USTRUCT(BlueprintType)
struct FDashSettings_Ylva
{
	GENERATED_BODY()


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
};

USTRUCT(BlueprintType)
struct FParrySettings_Ylva
{
	GENERATED_BODY()
	
	// The parry anim montage to play
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* ParryMontage;

	// The camera animation to play when parry has succeeded
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UCameraAnim* ParryCameraAnim;

	class UCameraAnimInst* ParryCameraAnimInst;

	// The parry window time frame
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float ParryWindowTime = 0.1f;

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

	// Settings that affect Ylva's dash behaviour
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FDashSettings_Ylva DashSettings;

	// Settings that affect Ylva's charge settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FChargeSettings_Ylva ChargeSettings;

	// Settings that affect blocking values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FDefenseSettings_Ylva BlockSettings;

	// Settings that affect parry values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FParrySettings_Ylva ParrySettings;

	// The amount of time (in seconds) the sword "sticks" when hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bEnableHitStop", ClampMin = 0.0f, ClampMax = 2.0f))
		float SwordStickTime = 0.1f;

	// Should the player rotate instantly where the camera is looking when attacking?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bRotateToCameraLookDirection : 1;
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

	void ApplyHitStop() override;

	// Returns CameraBoom component
	UFUNCTION(BlueprintCallable, Category = "Ylva | Components")
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera component
	UFUNCTION(BlueprintCallable, Category = "Ylva | Components")
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void StopMovement();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void ResumeMovement();

	// Returns true if we have taken 1 or more hits
	UFUNCTION(BlueprintCallable, Category = "Ylva | Combat")
		FORCEINLINE bool HasTakenAnyDamage() const { return HitCounter_Persistent > 0; }

	// Returns the light attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE float GetLightAttackDamage() const { return Combat.AttackSettings.LightAttackDamage; }

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE float GetHeavyAttackDamage() const { return Combat.AttackSettings.HeavyAttackDamage; }

	// Returns the charge attack damage value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE float GetChargeAttackDamage() const { return Combat.AttackSettings.ChargeAttackDamage; }

	// Returns the attack radius value
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

	// Returns the teleport radius value
	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		FORCEINLINE float GetTeleportRadius() const { return TeleportRadius; }

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

	// Returns true if we are currently locked on to the boss
	UFUNCTION(BlueprintPure, Category = "Ylva | Movement")
		bool IsLockedOn() const;

	// Increases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva | Charge Attack")
		void IncreaseCharge();

	// Turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float TurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float LookUpRate;

	// Ylva's debug options
	UPROPERTY(EditInstanceOnly, Category = "Ylva Debug")
		FDebug_Ylva Debug;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side/strafing input
	void MoveRight(float Value);

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

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;

	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void BroadcastLowStamina();

	void LoseHealth() override;
	void StartLosingHealth() override;

	void Die() override;

	float CalculateDirection() const;

	void CalculateRollLean(float DeltaTime);
	void CalculatePitchLean(float DeltaTime);

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

	// Called via input when holding down the charge attack key
	void ChargeUpAttack();

	// Called via input when releasing the charge attack key
	void ReleaseChargeAttack();

	// Called 0.2 seconds after ReleaseChargeAttack()
	void FinishChargeAttack();

	// Triggers the parry event to start
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void StartParryEvent();

	void FinishParryEvent();

	// Called via input to enter the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void Block();

	// Called via input to exit the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void StopBlocking();

	void BeginTakeDamage(float DamageAmount) override;

	void ApplyDamage(float DamageAmount) override;

	void EndTakeDamage() override;
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

	#pragma region Controls
	// Called via input to pause the game
	UFUNCTION(BlueprintCallable,Category = "Ylva | Controls")
		void Pause();

	// Disables the use of controller rotation for yaw axis
	UFUNCTION(BlueprintCallable,Category = "Ylva | Controls")
		void DisableControllerRotationYaw();
	#pragma endregion

	#pragma region LockOn
	// Called via input to toggle lock on mechanic
	UFUNCTION(BlueprintCallable,Category = "Ylva | Lock-On")
		void ToggleLockOn();

	// Utility function to enable lock on
	UFUNCTION(BlueprintCallable,Category = "Ylva | Lock-On")
		void EnableLockOn();

	// Utility function to disable lock on
	UFUNCTION(BlueprintCallable,Category = "Ylva | Lock-On")
		void DisableLockOn();
	#pragma endregion

	#pragma region Debug
	// Kill the player
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_Die();

	// Reset the player's achievements
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_ResetFeats();

	// Give the player full health
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_MaxHealth();

	// Give the player full stamina
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_RefillStamina();

	// Give the player full charge
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_MaxCharge();

	// Buff overall player stats
	UFUNCTION(BlueprintCallable, Category = "Ylva | Debug")
		void Debug_ToggleBuff();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable,Category = "Ylva | Debug")
		void ShowPlayerFSMVisualizer();

	// Called via input to show the Boss FSM Visualizer widget
	UFUNCTION(BlueprintCallable,Category = "Ylva | Debug")
		void ShowBossFSMVisualizer();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable,Category = "Ylva | Debug")
		void ShowMainHUD();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable,Category = "Ylva | Debug")
		void ShowNoHUD();
	#pragma endregion

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
	// Called when our health is below the given threshold
	void OnLowHealth() override;

	// Called when the boss's health is less than or equal to zero
	UFUNCTION()
		void OnBossDeath();

	// Called when our stamina value is below the given threshold
	UFUNCTION()
		void OnLowStamina();

	// Called when our combo tree has been reset
	UFUNCTION(BlueprintImplementableEvent)
		void OnComboReset();

	UFUNCTION()
		void OnComboReset_Implementation();

	// Called when our anim montage attack animations have finished
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnAttackEnd_Implementation(UAnimMontage* Montage, bool bInterrupted);

	// Called when we are light attacking
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnBeginLightAttack();
	
	// Called when we are heavy attacking
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnBeginHeavyAttack();

	// Called when we have successfully completed a light attack combo
	UFUNCTION(BlueprintImplementableEvent, Category = "Ylva | Combat")
		void OnLightAttackComboCompleted();
	#pragma endregion

	#pragma region Player States
		#pragma region Idle
		UFUNCTION()
			void OnEnterIdleState();
		UFUNCTION()
			void UpdateIdleState();
		UFUNCTION()
			void OnExitIdleState();
		#pragma endregion 

		#pragma region Walk
		UFUNCTION()
			void OnEnterWalkState();
		UFUNCTION()
			void UpdateWalkState();
		UFUNCTION()
			void OnExitWalkState();
		#pragma endregion 

		#pragma region Run
		UFUNCTION()
			void OnEnterRunState();
		UFUNCTION()
			void UpdateRunState();
		UFUNCTION()
			void OnExitRunState();
		#pragma endregion 

		#pragma region Shield Block
		UFUNCTION()
			void OnEnterBlockingState();
		UFUNCTION()
			void UpdateBlockingState();
		UFUNCTION()
			void OnExitBlockingState();
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

		#pragma region Shield Hit
		UFUNCTION()
			void OnEnterShieldHitState();
		UFUNCTION()
			void UpdateShieldHitState();
		UFUNCTION()
			void OnExitShieldHitState();
		#pragma endregion 

		#pragma region Dash
		UFUNCTION()
			void OnEnterDashState();
		UFUNCTION()
			void UpdateDashState();
		UFUNCTION()
			void OnExitDashState();
		#pragma endregion 

		#pragma region Parry
		UFUNCTION()
			void OnEnterParryState();
		UFUNCTION()
			void UpdateParryState();
		UFUNCTION()
			void OnExitParryState();
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
		UStaticMeshComponent* GetLeftHandSword();

	// Returns the sword static mesh components attached to the right hand bone
	UFUNCTION(BlueprintCallable,Category = "Ylva | Misc")
		UStaticMeshComponent* GetRightHandSword();
	#pragma endregion

	#pragma region Components
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// The camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UChargeAttackComponent* ChargeAttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UAttackComboComponent* AttackComboComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDashComponent* DashComponent;

	// This timeline plays when we are regenerating stamina
	FTimeline StaminaRegenTimeline;

	// This timeline plays when we are building charge
	FTimeline ChargeAttackTimeline;
	#pragma endregion

	// The radius the boss will use to teleport to a point on this circle
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Misc", meta = (ClampMin = 1.0f))
		float TeleportRadius = 1000.0f;

	// How long (in seconds) after the player's death, should we wait to respawn?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Misc", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float RespawnDelay = 1.8f;

	// The float curve to use when regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Stamina")
		class UCurveFloat* StaminaRegenCurve;

	// The float curve to use when building charge
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Charge Attack")
		class UCurveFloat* ChargeAttackCurve;

	// The minimum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMin = 30.0f;

	// The maximum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMax = 50.0f;

	// Ylva's movement settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", DisplayName = "Movement")
		FMovementSettings_Ylva MovementSettings;

	// Configure lock on settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat", DisplayName = "Lock-On")
		FLockOnSettings LockOnSettings;

	// Ylva's camera shake settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera")
		FCameraShakes_Ylva CameraShakes;

	// Ylva's combat settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		FCombatSettings_Ylva Combat;

	// Cached player's anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Ylva | Animation")
		class UYlvaAnimInstance* YlvaAnimInstance{};
private:
	TQueue<enum EAttackType> AttackQueue;
	TQueue<uint8> DashQueue;

	float LockedRightInput = 0.0f, LockedForwardInput = 0.0f;

	float PlayerLeanRollAmount = 0.0f;
	float PlayerLeanPitchAmount = 0.0f;

	uint8 bGodMode : 1;

	uint8 bIsRunKeyHeld : 1;
	uint8 bIsBuffed : 1;

	uint8 bWasLowStaminaEventTriggered : 1;

	uint8 HitCounter_Persistent = 0;

	uint16 ChargeAttackHoldFrames = 0;

	FTimerHandle ParryEventExpiryTimer;

	FTimerHandle ChargeAttackReleaseTimer;
	FTimerHandle ChargeLossTimerHandle;
	
	FTimerHandle AttackQueueTimerHandle;
	FTimerHandle AttackQueueExpiryTimerHandle;

	FTimerHandle DashQueueTimerHandle;

	class APlayerCameraManager* CameraManager;

	class UFeatData* UntouchableFeat;
};
