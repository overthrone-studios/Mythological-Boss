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

	void ChooseCombo();

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

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		float GetDistanceToPlayer() const;

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

	// The maximum movement speed while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float WalkSpeed = 300.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 1.0f, ClampMax = 10000.0f))
		float RunSpeed = 600.0f;

	// Should the boss wait before initiating the next combo?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat")
		uint8 bDelayBetweenCombo : 1;

	// The time in seconds to delay before starting a new combo
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float Delay = 0.1f;

	// Give or take a few seconds when delaying
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (EditCondition = "bDelayBetweenCombo", ClampMin = 0.0f, ClampMax = 10.0f))
		float RandomDeviation = 0.1f;

	// A list of combos the boss character will choose from
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.01f, ClampMax = 10.0f))
		TArray<UComboData*> Combos;

	// The amount of time in seconds this boss should be stunned for
	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 0.01f, ClampMax = 10.0f))
	//	float StunDuration = 0.3f;

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

	UAnimationAsset* HitAnimation{};

	FTimerHandle UpdateInfoTimerHandle;

	UComboData* ChosenCombo;

	//uint32 CurrentAttack = 0;

private:
	ACharacter* PlayerCharacter;
};
