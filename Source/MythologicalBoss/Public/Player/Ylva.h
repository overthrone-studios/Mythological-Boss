// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Ylva.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Ylva : public FCharacterDebug
{
	GENERATED_BODY()

	// Log the camera's pitch rotation value to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCameraPitch : 1;

	// Log the camera's pitch rotation value to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCurrentAnimTime : 1;

	// Draw the teleport circle around the player
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bShowTeleportRadius : 1;

	// Log the player's input values to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogPlayerInputValues : 1;

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

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 1000.0f, ClampMax = 10000.0f))
		float DashForce = 2000.0f;

	// The amount of time (in seconds) until we can dash again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float DashCooldown = 1.0f;

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
		FAttackSettings AttackSettings;

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
 * The player character you control
 */
UCLASS()
class MYTHOLOGICALBOSS_API AYlva final : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AYlva();

	void ApplyHitStop() override;

	// Returns CameraBoom component
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera component
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Returns true if we have used 1 or more health potions
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE bool HasUsedHealthPotion() const { return HealthPotionUses > 0; }

	// Returns true if we have taken 1 or more hits
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE bool HasTakenAnyDamage() const { return HitCounter_Persistent > 0; }

	// Returns the light attack damage value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetLightAttackDamage() const { return Combat.AttackSettings.LightAttackDamage; }

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetHeavyAttackDamage() const { return Combat.AttackSettings.HeavyAttackDamage; }

	// Returns the attack radius value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

	// Returns the teleport radius value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetTeleportRadius() const { return TeleportRadius; }

	// Returns true if we are light attacking
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		bool IsLightAttacking() const;

	// Returns true if we are heavy attacking
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		bool IsHeavyAttacking() const;

	// Increases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void IncreaseCharge();

	// Pause current animation, triggers a reset timer when called
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void PauseAnimsWithTimer();

	// Turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float TurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float LookUpRate;

	// Ylva's debug settings
	UPROPERTY(EditInstanceOnly, Category = "Ylva")
		FDebug_Ylva Debug;

	UPROPERTY(BlueprintReadWrite, Category = "Ylva")
		int32 HealthPotionUses = 0;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ChangeHitboxSize(float NewRadius) override;
	void UpdateCharacterInfo() override;
	void BroadcastLowHealth() override;

	void BroadcastLowStamina();

	void StartLosingHealth() override;
	void LoseHealth() override;

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

	void Die() override;

	void Debug_ResetFeats();

	// Give the player full health
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Debug_Die();

	// Give the player full health
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Debug_MaxHealth();

	// Give the player full stamina
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Debug_RefillStamina();

	// Give the player full charge
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Debug_MaxCharge();

	// Buff overall player stats
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Debug_ToggleBuff();

	// Called via input to toggle lock on mechanic
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ToggleLockOn();

	// Utility function to enable lock on
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void EnableLockOn();
	// Utility function to disable lock on
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DisableLockOn();

	// Called via input to enter the block state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Block();

	// Called via input to exit the block state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void StopBlocking();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void BeginLightAttack(class UAnimMontage* AttackMontage);
	
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void BeginHeavyAttack(class UAnimMontage* AttackMontage);

	// Called via input to enter the light attacking state
	void LightAttack();

	// Called via input to enter the heavy attacking state
	void HeavyAttack();

	// Called via input released to stop using controller rotation yaw
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DisableControllerRotationYaw();

	// Called via input to enter the running state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Run();

	// Called via input to exit the running state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void StopRunning();

	// Called via input to start dashing
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Dash();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ShowPlayerFSMVisualizer();

	// Called via input to show the Boss FSM Visualizer widget
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ShowBossFSMVisualizer();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ShowMainHUD();

	// Called via input to show the FSM Visualizer widget
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ShowNoHUD();

	// Called via input to pause the game
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Pause();

	// Called every frame
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void RegenerateStamina(float Rate);

	// Called when the player is killed (Health <= 0)
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void Respawn();

	// Set a new stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void SetStamina(float NewStaminaAmount);

	// Subtract stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DecreaseStamina(float Amount);

	// Add stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void IncreaseStamina(float Amount);

	// Resets stamina back to default
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetStamina();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void UpdateStamina(float StaminaToSubtract);

	UFUNCTION()
		void StartLosingStamina();

	UFUNCTION()
		void LoseStamina();

	UFUNCTION()
		void FinishLosingStamina();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void StartGainingCharge(float Amount);

	UFUNCTION()
		void GainCharge();

	UFUNCTION()
		void FinishGainingCharge();

	UFUNCTION()
		void UpdateIsRunHeld();

	// Resets global time dilation to 1
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetGlobalTimeDilation();

	// Did we successfully parry?
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		bool IsParrySuccessful();

	void StartParryEvent();
	void FinishParryEvent();

	// God mode functions
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void EnableGodMode();
	
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DisableGodMode();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ToggleGodMode();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void StartDashCooldown();

	// Attach sword to the right hand bone
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void AttachSword() const;

	// Detach from the right hand bone
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DetachSword();

	// Returns the sword static mesh components attached to the left hand bone
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		UStaticMeshComponent* GetLeftHandSword();

	// Returns the sword static mesh components attached to the right hand bone
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		UStaticMeshComponent* GetRightHandSword();

	// Decreases the charge meter
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DecreaseCharge();

	// Decreases the charge meter
	void DecreaseCharge(float Amount);

	// Resets the charge meter to 0
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetCharge();

	// Feats
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void OnWarriorFeatAchieved();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void OnUntouchableFeatAchieved();

	// Player states
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

	#pragma region Light Attack 1
	UFUNCTION()
		void OnEnterLightAttackState();
	UFUNCTION()
		void UpdateLightAttackState();
	UFUNCTION()
		void OnExitLightAttackState();
	#pragma endregion 

	#pragma region Light Attack 2
	UFUNCTION()
		void OnEnterLightAttack2State();
	UFUNCTION()
		void UpdateLightAttack2State();
	UFUNCTION()
		void OnExitLightAttack2State();
	#pragma endregion 

	#pragma region Heavy Attack 1
	UFUNCTION()
		void OnEnterHeavyAttackState();
	UFUNCTION()
		void UpdateHeavyAttackState();
	UFUNCTION()
		void OnExitHeavyAttackState();
	#pragma endregion 

	#pragma region Heavy Attack 2
	UFUNCTION()
		void OnEnterHeavyAttack2State();
	UFUNCTION()
		void UpdateHeavyAttack2State();
	UFUNCTION()
		void OnExitHeavyAttack2State();
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

	#pragma region Parry
	UFUNCTION()
		void OnEnterParryState();
	UFUNCTION()
		void UpdateParryState();
	UFUNCTION()
		void OnExitParryState();
	#pragma endregion 

	// Events
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnBossDeath(); // Called when the boss's health is less than or equal to zero

	void OnLowHealth() override;

	UFUNCTION()
		void OnLowStamina();

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

	// Toggle God mode?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva")
		uint8 bGodMode : 1;

	// The radius the boss will use to teleport to a point on this circle
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva", meta = (ClampMin = 1.0f))
		float TeleportRadius = 1000.0f;

	// This timeline plays when we are regenerating stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		class UTimelineComponent* StaminaRegenTimeline;

	// The float curve to use when regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stamina")
		class UCurveFloat* StaminaRegenCurve;

	// This timeline plays when we are building charge
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge Attack")
		class UTimelineComponent* ChargeAttackTimeline;

	// The float curve to use when building charge
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Charge Attack")
		class UCurveFloat* ChargeAttackCurve;

	// How long (in seconds) after the player's death, should we wait to respawn?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float RespawnDelay = 1.8f;

	// The minimum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMin = 30.0f;

	// The maximum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMax = 50.0f;

	// Ylva's movement settings
	UPROPERTY(EditInstanceOnly, Category = "Ylva", DisplayName = "Movement")
		FMovementSettings_Ylva MovementSettings;

	// Configure lock on settings
	UPROPERTY(EditInstanceOnly, Category = "Ylva", DisplayName = "Lock-On")
		FLockOnSettings LockOnSettings;

	// Ylva's camera shake settings
	UPROPERTY(EditInstanceOnly, Category = "Ylva")
		FCameraShakes_Ylva CameraShakes;

	// Ylva's combat settings
	UPROPERTY(EditInstanceOnly, Category = "Ylva Combat")
		FCombatSettings_Ylva Combat;

	// Right hand sword socket name
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva")
		FName R_SwordStartSocketName = "SwordSocket";

	// Left hand sword socket name
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva")
		FName L_SwordStartSocketName = "SwordSocket";

	// Cached player's anim instance, to control and trigger animations
	class UYlvaAnimInstance* YlvaAnimInstance{};

	// The right hand sword mesh
	UStaticMeshComponent* R_SwordMesh;
	// The left hand sword mesh
	UStaticMeshComponent* L_SwordMesh;

private:
	float InputX = 0.0f, InputZ = 0.0f;

	uint8 bIsRunKeyHeld : 1;
	uint8 bIsBuffed : 1;

	uint8 bWasLowStaminaEventTriggered : 1;

	uint8 HitCounter_Persistent = 0;

	FRotator StartRightSwordRotation{};
	FRotator StartLeftSwordRotation{};

	FTimerHandle DashCooldownTimer;
	FTimerHandle ParryEventExpiryTimer;

	FTimerHandle SwordDetachmentExpiryTimer;

	FTimerHandle ChargeLossTimerHandle;

	class APlayerCameraManager* CameraManager;
	ACharacter* Boss;

	class UFeatData* WarriorFeat;
	class UFeatData* UntouchableFeat;
};
