// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "TimerManager.h"
#include "OverthroneCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDebug
{
	GENERATED_BODY()

	// Allow raycasts to be drawn?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bShowRaycasts : 1;
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float WalkSpeed = 500.0f;
};

/**
 * Base class of an overthrone character
 */
UCLASS(Abstract)
class MYTHOLOGICALBOSS_API AOverthroneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOverthroneCharacter();

	virtual void AddDebugMessages();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ApplyHitStop();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void EnableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void DisableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void OnAttackLanded(const FHitResult& HitResult);

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		class UFSM* GetFSM() const { return FSM; }

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		virtual bool IsAttacking() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		virtual bool IsMovingInAnyDirection() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		bool IsDead() const;

	// Enable/Disable character from doing any actions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual bool IsLocked() const;

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		bool IsHealthZero() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void VibrateController(class UForceFeedbackEffect* ForceFeedbackToPlay = nullptr, bool bLooping = false, bool bIgnoreTimeDilation = true);

	void VibrateController(float Intensity = 1.0f, float Duration = 0.3f, bool bLeftSide = true, bool bRightSide = true);

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void StopVibrateController(class UForceFeedbackEffect* ForceFeedbackToStop);

	// Returns the recent damage value
	UFUNCTION(BlueprintPure, Category = "Overthrone Character | Combat")
		float GetRecentDamage() const;

	// Hit stop functions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void PauseAnims() const;
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void UnPauseAnims() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character | Combat")
		virtual void ApplyKnockbackEffect();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	// Send this character's tracked information to the game state
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void UpdateCharacterInfo();

	UFUNCTION(BlueprintPure, Category = "Overthrone Character")
		virtual float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void BeginTakeDamage(float DamageAmount, const FDamageEvent& DamageEvent);
	
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ApplyDamage(float DamageAmount, const FDamageEvent& DamageEvent);
	
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void EndTakeDamage(const FDamageEvent& DamageEvent);

	// Enable/Disable character from doing any actions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ToggleLockSelf();

	// Disable character from doing any actions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void LockSelf();

	// Unlock character from the locked state
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void UnlockSelf();

	// Called when we are dead
	UFUNCTION(BlueprintImplementableEvent)
		void OnDeath();

	// Set a new health value
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void SetHealth(float NewHealthAmount);

	// Add health by an amount
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void IncreaseHealth(float Amount);

	// Subtract health by an amount
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void DecreaseHealth(float Amount);

	// Resets health back to default
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void ResetHealth();

	// Update the character's health
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void UpdateHealth(float HealthToSubtract);

	// Kill self
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual bool IsInvincible() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void StopMovement();

	// Changes the hit box dimensions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ChangeHitboxSize(float NewRadius);

	#pragma region Overthrone Character Base States
		#pragma region Death
			UFUNCTION()
				virtual void OnEnterDeathState();
			
			UFUNCTION()
				virtual void UpdateDeathState(float Uptime, int32 Frames);
			
			UFUNCTION()
				virtual void OnExitDeathState();
		#pragma endregion

		#pragma region Locked
			UFUNCTION()
				virtual void OnEnterLockedState();
			
			UFUNCTION()
				virtual void UpdateLockedState(float Uptime, int32 Frames);
			
			UFUNCTION()
				virtual void OnExitLockedState();
		#pragma endregion
	#pragma endregion

	UFUNCTION()
		virtual void DoKnockback();

	UFUNCTION()
		virtual void OnFinishedKnockback();

	// Called when the character's health is less than or equal to the low health threshold
	UFUNCTION()
		virtual void OnLowHealth();

	// Called when the character has regained health after being in low health state
	UFUNCTION()
		virtual void OnExitLowHealth();

	// Prep work before playing the timeline to lose health
	UFUNCTION()
		virtual void StartLosingHealth();

	// Called every tick of the take damage timeline
	UFUNCTION()
		virtual void LoseHealth();

	// Called when the timeline has finished playing
	UFUNCTION()
		virtual void FinishLosingHealth();

	// Called when performing an action montage while in the "Anticipation" section
	UFUNCTION(BlueprintImplementableEvent, Category = "Overthrone Character | Combat")
		void OnAnticipationSection();
	
	// Called when performing an action montage while in the "Contact" section
	UFUNCTION(BlueprintImplementableEvent, Category = "Overthrone Character | Combat")
		void OnContactSection();
	
	// Called when performing an action montage while in the "Recovery" section
	UFUNCTION(BlueprintImplementableEvent, Category = "Overthrone Character | Combat")
		void OnRecoverySection();

	// Send out a signal to the functions that have binded to the OnLowHealth event
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void BroadcastLowHealth();

	// Send out a signal to the functions that have binded to the OnExitLowHealth event
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void BroadcastExitLowHealth();

	// The character's finite state machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		class UFSM* FSM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		class UHealthComponent* HealthComponent;

	// The float curve to use when taking damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		class UCurveFloat* HealthLossCurve;

	// The float curve to use when being knocked back
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overthrone Character | Combat")
		UCurveFloat* KnockbackCurve;

	// The skeletal mesh representing the character
	USkeletalMesh* SkeletalMesh;

	// Tracks the amount of hits we've taken
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		uint8 HitCounter = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		uint8 bIsDead : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		uint8 bIsLocked : 1;

	// Cached world pointer
	UWorld* World{};

	// A set of all of our child components attached to this actor
	TSet<UActorComponent*> Components;

	// Cached player controller pointer
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		APlayerController* PlayerController{};

	// Cached game instance pointer
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UOverthroneGameInstance* GameInstance{};

	// Cached game state pointer
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class AOverthroneGameState* GameState{};

	// Cached movement component
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UCharacterMovementComponent* MovementComponent{};

	// Cached capsule component
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UCapsuleComponent* CapsuleComp;

	// Cached character's anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UOverthroneAnimInstance* AnimInstance{};

	// Cached Overthrone HUD reference, used to access menus
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class AOverthroneHUD* OverthroneHUD{};

	// To access the main HUD
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UMainPlayerHUD* MainHUD{};

	// To give data to the Visualizer HUD
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		class UFSMVisualizerHUD* FSMVisualizer{};

	// To access sockect locations
	class USkeletalMeshComponent* SKMComponent;

	// A cache to the timer manager object
	FTimerManager* TimerManager;

	FTimerHandle DeathExpiryTimerHandle;
	FTimerHandle HitStopTimerHandle;

	// These timelines play when we have taken damage
	FTimeline TL_HealthLoss;
	FTimeline TL_Knockback;

	uint8 bWasLowHealthEventTriggered : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		uint8 LandedHits = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		float RecentDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		FRotator ControlRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		FVector CurrentLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		FRotator CurrentRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		FVector ForwardVector;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		FVector RightVector;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		float ForwardInput = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		float RightInput = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Character")
		float CurrentMovementSpeed = 0.0f;
};
