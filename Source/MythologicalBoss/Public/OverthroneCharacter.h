// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "OverthroneCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDebug
{
	GENERATED_BODY()

	// Log our health values to the viewport?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogHealthValues : 1;

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

USTRUCT(BlueprintType)
struct FCameraShakeData
{
	GENERATED_BODY()

	// The camera shake to play
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TSubclassOf<class UCameraShake> Shake;

	// The intensity of the camera shake
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float Intensity = 1.0f;
};

USTRUCT(BlueprintType)
struct FCameraShakes
{
	GENERATED_BODY()

	// The camera shake to play when we are damaged by the boss
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Damaged;
};

USTRUCT(BlueprintType)
struct FAttackSettings
{
	GENERATED_BODY()

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float AttackRadius = 20.0f;

	// The radius of the sphere raycast when attacking light or heavy on low health
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float AttackRadiusOnLowHealth = 40.0f;
};

USTRUCT(BlueprintType)
struct FCombatSettings
{
	GENERATED_BODY()

	// Should we stop animations when we are hit?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bEnableHitStop : 1;

	// The amount of time (in seconds) we stay paused when we hit something
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.001f, ClampMax = 10.0f, EditCondition = "bEnableHitStop"))
		float HitStopTime = 0.1f;
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

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ApplyHitStop();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	virtual void UpdateCharacterInfo();

	void InitTimelineComponent(class UTimelineComponent* InTimelineComponent, class UCurveFloat* InCurveFloat, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName);

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

	// Hit stop functions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void PauseAnims() const;
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void UnPauseAnims() const;

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		bool IsInvincible() const;

	// Changes the hit box dimensions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void ChangeHitboxSize(float NewRadius);

	// Called when the character's health is less than or equal to the low health threshold
	UFUNCTION()
		virtual void OnLowHealth();

	// Prep work before playing the timeline to lose health
	UFUNCTION()
		virtual void StartLosingHealth(float Amount);

	// Called every tick of the take damage timeline
	UFUNCTION()
		virtual void LoseHealth();

	// Called when the timeline has finished playing
	UFUNCTION()
		virtual void FinishLosingHealth();

	// Send out a signal to the functions that have binded to the OnLowHealth event
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		virtual void BroadcastLowHealth();

	// The character's finite state machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		class UFSM* FSM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		class UHealthComponent* HealthComponent;

	// This timeline plays when we have taken damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		class UTimelineComponent* HealthLossTimeline;

	// The float curve to use when taking damage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health")
		class UCurveFloat* HealthLossCurve;

	// The skeletal mesh representing the character
	USkeletalMesh* SkeletalMesh;

	// True when we have been damaged
	uint8 bIsHit : 1;

	// Tracks the amount of hits we've taken
	uint8 HitCounter = 0;

	// Cached world pointer
	UWorld* World{};

	// A set of all of our child components attached to this actor
	TSet<UActorComponent*> Components;

	// Cached player controller pointer
	APlayerController* PlayerController{};

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached character's anim instance, to control and trigger animations
	class UOverthroneAnimInstance* AnimInstance{};

	// Cached Overthrone HUD reference, used to access menus
	class AOverthroneHUD* OverthroneHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	FTimerHandle DeathExpiryTimerHandle;
	FTimerHandle HitStopTimerHandle;

	uint8 bWasLowHealthEventTriggered : 1;
};
