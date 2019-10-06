// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Queue.h"
#include "Components/AttackComboComponent.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bLockedOn : 1;

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

	// The amount of time (in seconds) the attack queue will clear itself. If the queue takes longer than the specified time (in seconds), clear the queue.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float CloseRangeAttackRotationSpeed = 5.0f;

	// The rotation speed the character will rotate at to face the boss when in the super close range
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float SuperCloseRangeAttackRotationSpeed = 2.0f;
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
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void StopMovement();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void ResumeMovement();

	UFUNCTION(BlueprintCallable, Category = "Ylva | Movement")
		void SpawnGhost();

	// Returns true if we have taken 1 or more hits
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
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
	UFUNCTION(BlueprintPure, Category = "Ylva | Combat")
		bool IsLowStamina() const;

	// Returns true if we are currently locked on to the boss
	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		bool IsLockedOn() const;

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

	// Increases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva | Charge Attack")
		void IncreaseCharge();

	UFUNCTION(BlueprintPure, Category = "Ylva | Feedback")
		class UForceFeedbackEffect* GetCurrentForceFeedback() const;

	bool HasMovedRightBy(float Distance) override;

	bool HasMovedLeftBy(float Distance) override;

	void OnAttackLanded() override;

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

	void LockOnTo(const FVector& TargetLocation, float DeltaTime);

	void FaceBoss(float DeltaTime, float RotationSpeed = 10.0f);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;

	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void BroadcastLowStamina();

	void Die() override;

	float CalculateDirection() const;

	void CalculateRollLean(float DeltaTime);
	void CalculatePitchLean(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Ylva | Misc")
		float GetDistanceToBoss() const;

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

	UFUNCTION()
		void OnParryBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called via input to enter the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void Block();

	// Called via input to exit the block state
	UFUNCTION(BlueprintCallable,Category = "Ylva | Combat")
		void StopBlocking();

	void BeginTakeDamage(float DamageAmount) override;

	void ApplyDamage(float DamageAmount, const FDamageEvent& DamageEvent) override;

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

	// Toggle animation blend out on low stamina
	void Debug_ToggleLowStaminaAnimBlendOut();

	// Called via input to trigger boss stage 1 behaviour
	void Debug_BossStage1();
	
	// Called via input to trigger boss stage 2 behaviour
	void Debug_BossStage2();
	
	// Called via input to trigger boss stage 3 behaviour
	void Debug_BossStage3();

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
	// Called when our health is below the given threshold
	void OnLowHealth() override;

	// Called when the boss's health is less than or equal to zero
	UFUNCTION(BlueprintImplementableEvent)
		void OnBossDeath();

	UFUNCTION()
		void OnBossDeath_Implementation();

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
	#pragma endregion

	UFUNCTION()
		void OnEnterAnyState();
	UFUNCTION()
		void UpdateAnyState();
	UFUNCTION()
		void OnExitAnyState();

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

		#pragma region Charge Attack
		UFUNCTION()
			void OnEnterChargeAttackState();
		UFUNCTION()
			void UpdateChargeAttackState();
		UFUNCTION()
			void OnExitChargeAttackState();
		#pragma endregion 

		#pragma region Shocked
		UFUNCTION()
			void OnEnterShockedState();
		UFUNCTION()
			void UpdateShockedState();
		UFUNCTION()
			void OnExitShockedState();
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
		class UCameraComponent* FollowCamera;

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

	// The radius the boss will use to teleport to a point on this circle
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Misc", meta = (ClampMin = 1.0f))
		float TeleportRadius = 1000.0f;

	// How long (in seconds) after the player's death, should we wait to respawn?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Misc", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float RespawnDelay = 1.8f;

	// The float curve to use when regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina")
		class UCurveFloat* StaminaRegenCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin=0.0f))
		uint8 bEnableBlendOutOnLowStamina : 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin=0.0f, EditCondition = "bEnableBlendOutOnLowStamina"))
		float BlendOutTimeOnLowStamina = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin=0.0f, EditCondition = "bEnableBlendOutOnLowStamina"))
		float BlendOutTriggerTimeOnLowStamina = 0.2f;

	// The float curve to use when building charge
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Charge Attack")
		class UCurveFloat* ChargeAttackCurve;

	// The minimum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMin = 30.0f;

	// The maximum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMax = 50.0f;

	// The minimum pitch rotation value (in degrees) the lock on camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = "-90.0", ClampMax = 90.0f))
		float LockOnPitchMin = 5.0f;

	// The maximum pitch rotation value (in degrees) the lock on camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float LockOnPitchMax = 30.0f;

	// The rotation speed of the lock on camera (1 = Slow, 10+ = Fast)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera", meta = (ClampMin = 0.0f))
		float LockOnRotationSpeed = 10.0f;

	// Ylva's movement settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", DisplayName = "Movement")
		FMovementSettings_Ylva MovementSettings;

	// Configure lock on settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat", DisplayName = "Lock-On")
		FLockOnSettings LockOnSettings;

	// The amount of time (in seconds) we are electrecuted for when we are hit by a lightning strike
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat", DisplayName = "Electric Shock Time")
		float ShockTime = 2.0f;

	// Ylva's camera shake settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera")
		FCameraShakes_Ylva CameraShakes;

	// Ylva's combat settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Combat")
		FCombatSettings_Ylva Combat;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", DisplayName = "Ghost Blueprint Class")
		TSubclassOf<AActor> GhostClass;

	// Cached player's anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Ylva | Animation")
		class UYlvaAnimInstance* YlvaAnimInstance{};

private:
	TQueue<EAttackType_Player> AttackQueue;
	TQueue<uint8> DashQueue;

	FVector RightMovementStart, RightMovementEnd;
	float DistanceMovedInRightDirection;

	FRotator ControlRotation;
	FRotator DirectionToBoss;
	float DistanceToBoss;

	float PlayerLeanRollAmount = 0.0f;
	float PlayerLeanPitchAmount = 0.0f;

	uint8 MaxDashInQueue = 1;

	uint8 bChargeKeyPressed : 1;
	uint8 ChargeKeyHeldFrames = 0;

	uint8 bWasRunning : 1;

	uint8 bGodMode : 1;
	uint8 bCanDash : 1;

	uint8 bIsParryBoxHit : 1;

	uint8 bHasBeenDamaged : 1;

	uint8 bCanRun : 1;

	uint8 bIsRunKeyHeld : 1;
	uint8 bIsBuffed : 1;

	uint8 bWasLowStaminaEventTriggered : 1;

	uint8 HitCounter_Persistent = 0;

	uint16 ChargeAttackHoldFrames = 0;

	FTimerHandle TH_ParryEventExpiry;

	FTimerHandle TH_ChargeAttackRelease;
	FTimerHandle TH_ChargeLoss;
	
	FTimerHandle TH_AttackQueue;
	FTimerHandle TH_AttackQueueExpiry;

	FTimerHandle TH_DashQueue;

	class USwordComponent* LSword;
	class USwordComponent* RSword;

	class UForceFeedbackEffect* CurrentForceFeedback;

	float PreviousAttackMontageBlendOutTime = 0.25f;
	float PreviousAttackMontageBlendOutTriggerTime = 0.1f;

	class APlayerCameraManager* CameraManager;

	class UFeatData* UntouchableFeat;
};
