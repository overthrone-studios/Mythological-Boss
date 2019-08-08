// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "Ylva.generated.h"

/*
 * The player character
 */
UCLASS()
class MYTHOLOGICALBOSS_API AYlva final : public ACharacter
{
	GENERATED_BODY()

public:
	AYlva();

	// Returns CameraBoom component
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera component
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetLightAttackDamage() const { return LightAttackDamage; }
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetHeavyAttackDamage() const { return HeavyAttackDamage; }
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetAttackRange() const { return AttackDistance; }
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// Turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float TurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float LookUpRate;

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

	// Called via input to enter the light attacking state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void LightAttack();

	// Called via input to enter the heavy attacking state
	UFUNCTION(BlueprintCallable, Category = "Ylva")
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
		void ShowFSMVisualizer();

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

	// Set a new health value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void SetHealth(float NewHealthAmount);

	// Set a new stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void SetStamina(float NewStaminaAmount);

	// Update health value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void UpdateHealth(float AmountToSubtract);

	// Update stamina value
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void UpdateStamina(float AmountToSubtract);

	// Resets stamina back to default
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetStamina();

	// Resets health back to default
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetHealth();

	// Resets global time dilation to 1
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ResetGlobalTimeDilation();

	void StartParryEvent(); 
	void FinishParryEvent(); 

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

	#pragma region Fall
	UFUNCTION()
		void OnEnterFallingState();
	UFUNCTION()
		void UpdateFallingState();
	UFUNCTION()
		void OnExitFallingState();
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

	// Called when the boss's health is less than or equal to zero
	UFUNCTION()
		void OnBossDeath();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void EnableGodMode();
	
	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void DisableGodMode();

	UFUNCTION(BlueprintCallable, Category = "Ylva")
		void ToggleGodMode();

	// The skeletal mesh representing the player
	USkeletalMesh* SkeletalMesh;

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// The camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// The player's Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ylva")
		class UFSM* PlayerStateMachine;

	// Toggle God mode?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva")
		uint8 bGodMode : 1;

	// The player's starting health
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva", meta = (ClampMin = 100.0f, ClampMax = 10000.0f))
		float StartingHealth = 100.0f;

	// The player's current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ylva")
		float Health = 100.0f;

	// The player's starting stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva", meta = (ClampMin = 100.0f, ClampMax = 10000.0f))
		float StartingStamina = 100.0f;

	// The player's current stamina level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ylva")
		float Stamina = 100.0f;

	// How long (in seconds) after the player's death, should we wait to respawn?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float RespawnDelay = 1.8f;

	// The rate of stamina regeneration
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float StaminaRegenerationRate = 10.0f;

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float WalkSpeed = 300.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float RunSpeed = 600.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Movement", meta = (ClampMin = 1000.0f, ClampMax = 10000.0f))
		float DashForce = 2000.0f;

	// Should the camera look towards the boss?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 10.0f, ClampMax = 1000.0f))
		bool bShouldLockOnTarget = false;

	// The target pitch when locking on
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 270.0f, ClampMax = 360.0f))
		float LockOnPitch = 350.0f;

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float AttackDistance = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 1.0f, ClampMax = 1000.0f))
		float AttackRadius = 10.0f;

	// The stamina value to subtract when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float LightAttackStamina = 10.0f;

	// The stamina value to subtract when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float HeavyAttackStamina = 30.0f;

	// The stamina value to subtract when dashing
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float DashStamina = 40.0f;

	// The stamina value to subtract while running
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float RunStamina = 2.0f;

	// The stamina value to subtract when being hit while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float ShieldHitStamina = 10.0f;

	// This value will be used to buffer the damage received when the boss hits the player while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float DamageBuffer = 0.5f;

	// The parry window time frame
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float ParryWindowTime = 0.3f;

	// This value will be used when a parry is successful (slowing down time)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float TimeDilationOnSuccessfulParry = 0.4f;

	// How long (in seconds) should we stay in the parry event?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float TimeUntilParryEventIsCompleted = 0.5f;

	// A camera shake that plays when the player has taken damage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat")
		TSubclassOf<class UCameraShake> Damaged;

	// The intensity of the camera shake
	UPROPERTY(EditInstanceOnly, Category = "Ylva Combat", meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float ShakeIntensity = 1.0f;

	// Cached world pointer
	UWorld* World{};

	// Cached player controller pointer
	APlayerController* PlayerController{};

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached player's anim instance, to control and trigger animations
	class UYlvaAnimInstance* AnimInstance{};

	// Cached Overthrone HUD reference, used to access menus
	class AOverthroneHUD* OverthroneHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	FTimerHandle DeathStateExpiryTimer;
	FTimerHandle ParryEventExpiryTimer;
};
