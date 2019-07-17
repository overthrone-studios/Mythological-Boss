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

	// Turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float TurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float LookUpRate;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void OnJumped_Implementation() override;
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

	// Called via input to enter the block state
	void Block();

	// Called via input to exit the block state
	void StopBlocking();

	// Called via input to enter the attacking state
	void Attack();

	UFUNCTION()
		void OnEnterIdleState();
	UFUNCTION()
		void UpdateIdleState();
	UFUNCTION()
		void OnExitIdleState();

	UFUNCTION()
		void OnEnterWalkState();
	UFUNCTION()
		void UpdateWalkState();
	UFUNCTION()
		void OnExitWalkState();

	UFUNCTION()
		void OnEnterBlockingState();
	UFUNCTION()
		void UpdateBlockingState();
	UFUNCTION()
		void OnExitBlockingState();

	UFUNCTION()
		void OnEnterAttackState();
	UFUNCTION()
		void UpdateAttackState();
	UFUNCTION()
		void OnExitAttackState();

	UFUNCTION()
		void OnEnterJumpState();
	UFUNCTION()
		void UpdateJumpState();
	UFUNCTION()
		void OnExitJumpState();

	UFUNCTION()
		void OnEnterFallingState();
	UFUNCTION()
		void UpdateFallingState();
	UFUNCTION()
		void OnExitFallingState();

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

	// Cached world pointer
	UWorld* World{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	class AOverthroneHUD* PlayerHUD{};
	class UFSMVisualizerHUD* FSMVisualizer{};
};
