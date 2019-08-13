// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "OverthroneCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDebug
{
	GENERATED_BODY()


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

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float AttackDistance = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float AttackRadius = 20.0f;
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

	// Settings that affect the character's attack values
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FAttackSettings AttackSettings;
};

UCLASS(Abstract)
class MYTHOLOGICALBOSS_API AOverthroneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOverthroneCharacter();
	
	// Returns the light attack damage value
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		FORCEINLINE float GetLightAttackDamage() const { return Combat.AttackSettings.LightAttackDamage; }

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		FORCEINLINE float GetHeavyAttackDamage() const { return Combat.AttackSettings.HeavyAttackDamage; }

	// Returns the attack distance value
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		FORCEINLINE float GetAttackRange() const { return Combat.AttackSettings.AttackDistance; }

	// Returns the attack radius value
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		FORCEINLINE float GetAttackRadius() const { return Combat.AttackSettings.AttackRadius; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	virtual void UpdateCharacterInfo();

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

	// Kill self
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void Die();

	// Hit stop functions
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void PauseAnims();
	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		void UnPauseAnims();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Character")
		bool IsInvincible();

	// The character's finite state machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		class UFSM* FSM;

	// The player's starting health
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Overthrone Character", meta = (ClampMin = 100.0f, ClampMax = 10000.0f))
		float StartingHealth = 100.0f;

	// The player's current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overthrone Character")
		float Health = 100.0f;

	// The character's combat settings
	UPROPERTY(EditInstanceOnly, Category = "Overthrone Character", DisplayName = "Movement")
		FMovementSettings MovementSettings;

	// The character's combat settings
	UPROPERTY(EditInstanceOnly, Category = "Overthrone Character")
		FCombatSettings Combat;

	// The character's camera shake settings
	UPROPERTY(EditInstanceOnly, Category = "Overthrone Character")
		FCameraShakes CameraShakes;

	// The character's camera shake settings
	UPROPERTY(EditInstanceOnly, Category = "Overthrone Character")
		FCharacterDebug Debug;

	// The skeletal mesh representing the character
	USkeletalMesh* SkeletalMesh;

	// True when we have been damaged
	uint8 bIsHit : 1;

	// Cached world pointer
	UWorld* World{};

	// Cached player controller pointer
	APlayerController* PlayerController{};

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached player's anim instance, to control and trigger animations
	//class UYlvaAnimInstance* AnimInstance{}; todo UOverthroneAnimInstance

	// Cached Overthrone HUD reference, used to access menus
	class AOverthroneHUD* OverthroneHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	FTimerHandle DeathExpiryTimerHandle;
	FTimerHandle HitStopTimerHandle;
};
