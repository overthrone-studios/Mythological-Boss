// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "Combat/ComboData.h"
#include "Mordath.generated.h"

UCLASS()
class MYTHOLOGICALBOSS_API AMordath final : public ACharacter
{
	GENERATED_BODY()

public:
	AMordath();

	class UBehaviorTree* GetBT() const { return BossBT; }

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		class UFSM* GetFSM() const { return BossStateMachine; }

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetLightAttackDamage() const { return LightAttackDamage; }
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetHeavyAttackDamage() const { return HeavyAttackDamage; }
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetAttackRange() const { return AttackDistance; }
	UFUNCTION(BlueprintCallable, Category = "Mordath")
		FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FRotator FacePlayer();
	void SendInfo();
	bool ShouldDestroyDestructibleObjects();

	void ChooseComboWithDelay();
	void ChooseCombo();

	void FinishStun();

	#pragma region Idle
	UFUNCTION()
		void OnEnterIdleState();
	UFUNCTION()
		void UpdateIdleState();
	UFUNCTION()
		void OnExitIdleState();
	#pragma endregion 

	#pragma region Follow
	UFUNCTION()
		void OnEnterFollowState();
	void ChooseAttack();
	UFUNCTION()
		void UpdateFollowState();
	UFUNCTION()
		void OnExitFollowState();
	#pragma endregion 

	#pragma region Light Attack 1
	UFUNCTION()
		void OnEnterLightAttack1State();
	UFUNCTION()
		void UpdateLightAttack1State();
	UFUNCTION()
		void OnExitLightAttack1State();
	#pragma endregion 

	#pragma region Light Attack 2
	UFUNCTION()
		void OnEnterLightAttack2State();
	UFUNCTION()
		void UpdateLightAttack2State();
	UFUNCTION()
		void OnExitLightAttack2State();
	#pragma endregion 

	#pragma region Light Attack 3
	UFUNCTION()
		void OnEnterLightAttack3State();
	UFUNCTION()
		void UpdateLightAttack3State();
	UFUNCTION()
		void OnExitLightAttack3State();
	#pragma endregion 

	#pragma region Heavy Attack 1
	UFUNCTION()
		void OnEnterHeavyAttack1State();
	UFUNCTION()
		void UpdateHeavyAttack1State();
	UFUNCTION()
		void OnExitHeavyAttack1State();
	#pragma endregion 

	#pragma region Heavy Attack 2
	UFUNCTION()
		void OnEnterHeavyAttack2State();
	UFUNCTION()
		void UpdateHeavyAttack2State();
	UFUNCTION()
		void OnExitHeavyAttack2State();
	#pragma endregion 

	#pragma region Heavy Attack 3
	UFUNCTION()
		void OnEnterHeavyAttack3State();
	UFUNCTION()
		void UpdateHeavyAttack3State();
	UFUNCTION()
		void OnExitHeavyAttack3State();
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

	#pragma region Stunned
	UFUNCTION()
		void OnEnterStunnedState();
	UFUNCTION()
		void UpdateStunnedState();
	UFUNCTION()
		void OnExitStunnedState();
	#pragma endregion 

	#pragma region Laugh
	UFUNCTION()
		void OnEnterLaughState();
	UFUNCTION()
		void UpdateLaughState();
	UFUNCTION()
		void OnExitLaughState();
	#pragma endregion 

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		float GetDistanceToPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void EnableInvincibility();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void DisableInvincibility();

	// Called when the player's health is less than or equal to 0
	UFUNCTION()
		void OnPlayerDeath();

	void DestroySelf();

	// The skeletal mesh representing the player
	USkeletalMesh* SkeletalMesh;

	class UBehaviorTree* BossBT;

	class ABossAIController* BossAIController{};

	// The player's Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UFSM* BossStateMachine;

	// The starting health of the boss
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 100.0f, ClampMax = 100000.0f))
		float StartingHealth = 1000.0f;

	// The current health of the boss
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 100.0f, ClampMax = 100000.0f))
		float Health = 1000.0f;

	// The radius in which the boss character will accept that it has arrived to the player's location
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 1.0f, ClampMax = 100000.0f))
		float AcceptanceRadius = 200.0f;

	// How long (in seconds) should the boss stay dead before being destroyed?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 0.01f, ClampMax = 100000.0f))
		float DeathTime = 2.0f;

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float WalkSpeed = 600.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float RunSpeed = 1000.0f;

	// Should the boss wait before initiating the next combo?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat")
		uint8 bDelayBetweenCombo : 1;

	// The time in seconds to delay before choosing a new combo
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float ComboDelayTime = 1.0f;

	// Adds a random range to ComboDelayTime
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float RandomDeviation = 0.1f;

	// Should the boss choose a random combo from the Combos list?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat")
		uint8 bChooseRandomCombo : 1;

	// A list of combos the boss character will choose from
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.01f, ClampMax = 10.0f))
		TArray<UComboData*> Combos;

	// The amount of time in seconds this boss should be stunned for
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.01f, ClampMax = 10.0f))
		float StunDuration = 0.8f;

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float AttackDistance = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 1.0f, ClampMax = 1000.0f))
		float AttackRadius = 10.0f;

	// The distance to ray cast from the boss's location (for destructible actor detection)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 1.0f, ClampMax = 1000.0f))
		float BoxDetectionDistance = 130.0f;

	// Maximum hits that can be taken before becoming invicible
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0, ClampMax = 100))
		uint8 MaxHitsBeforeInvincibility = 3;

	// The amount of time (in seconds) that the boss can stay invincible after being damaged by the player
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.01f, ClampMax = 100.0f))
		float InvincibilityTimeAfterDamage = 1.5f;

	int8 ComboIndex = 0; // This is used to choose a random index in the combos list

	uint8 bCanAttack : 1; // Used to prevent attacking when player is dead
	uint8 HitCounter = 0;

	// Cached world pointer
	UWorld* World{};

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached boss's anim instance, to control and trigger animations
	class UMordathAnimInstance* AnimInstance{};

	// Cached Overthrone HUD reference, used to access menus
	class AOverthroneHUD* OverthroneHUD{};

	class UMainPlayerHUD* PlayerHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	UComboData* ChosenCombo;

	TArray<UComboData*> CachedCombos;

private:
	FTimerHandle UpdateInfoTimerHandle;
	FTimerHandle ComboDelayTimerHandle;
	FTimerHandle InvincibilityTimerHandle;
	FTimerHandle StunExpiryTimerHandle;
	FTimerHandle DeathExpiryTimerHandle;

	ACharacter* PlayerCharacter;
};
