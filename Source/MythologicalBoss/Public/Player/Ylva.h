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
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera component
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE float GetLightAttackDamage() const { return LightAttackDamage; }
	FORCEINLINE float GetHeavyAttackDamage() const { return HeavyAttackDamage; }
	FORCEINLINE float GetAttackRange() const { return AttackDistance; }
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

	void Landed(const FHitResult& Hit) override;

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

	// Called via input to enter the jumping state
	void StartJumping();

	// Called via input to enter the block state
	void Block();

	// Called via input to exit the block state
	void StopBlocking();

	// Called via input to enter the light attacking state
	void LightAttack();

	// Called via input to enter the heavy attacking state
	void HeavyAttack();

	// Called via input released to stop using controller rotation yaw
	void DisableControllerRotationYaw();

	// Called via input to enter the running state
	void Run();

	// Called via input to exit the running state
	void StopRunning();

	// Called via input to show the FSM Visualizer widget
	void ShowFSMVisualizer();

	// Called via input to show the Boss FSM Visualizer widget
	void ShowBossFSMVisualizer();

	// Called via input to show the FSM Visualizer widget
	void ShowMainHUD();

	// Called via input to show the FSM Visualizer widget
	void ShowNoHUD();

	// Called via input to pause the game
	void Pause();

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

	#pragma region Jump
	UFUNCTION()
		void OnEnterJumpState();
	UFUNCTION()
		void UpdateJumpState();
	UFUNCTION()
		void OnExitJumpState();
	#pragma endregion 

	#pragma region Fall
	UFUNCTION()
		void OnEnterFallingState();
	UFUNCTION()
		void UpdateFallingState();
	UFUNCTION()
		void OnExitFallingState();
	#pragma endregion 

	#pragma region Taunt 1
		UFUNCTION()
			void OnEnterTauntState();
		UFUNCTION()
			void UpdateTauntState();
		UFUNCTION()
			void OnExitTauntState();
	#pragma endregion 

	// Events
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
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

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", meta = (ClampMin=10.0f, ClampMax=10000.0f))
		float WalkSpeed = 300.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Movement", meta = (ClampMin=10.0f, ClampMax=10000.0f))
		float RunSpeed = 600.0f;

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin=10.0f, ClampMax=10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin=10.0f, ClampMax=10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin=10.0f, ClampMax=10000.0f))
		float AttackDistance = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Combat", meta = (ClampMin=10.0f, ClampMax=1000.0f))
		float AttackRadius = 10.0f;

	// Cached world pointer
	UWorld* World{};

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
};
