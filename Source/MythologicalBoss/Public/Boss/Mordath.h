// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
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

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SendInfo();

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

	// The skeletal mesh representing the player
	USkeletalMesh* SkeletalMesh;

	class UBehaviorTree* BossBT;

	class ABossAIController* BossAIController;

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
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float WalkSpeed = 300.0f;

	// The maximum movement speed while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Movement", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float RunSpeed = 600.0f;

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The attack range when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 10.0f, ClampMax = 10000.0f))
		float AttackDistance = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mordath Combat", meta = (ClampMin = 10.0f, ClampMax = 1000.0f))
		float AttackRadius = 10.0f;

	// Cached world pointer
	UWorld* World{};

	// Cached game instance pointer
	class UOverthroneGameInstance* GameInstance{};

	// Cached movement component
	UCharacterMovementComponent* MovementComponent{};

	// Cached Overthrone HUD reference, used to access menus
	class AOverthroneHUD* OverthroneHUD{};

	class UMainPlayerHUD* PlayerHUD{};

	// To give data to the Visualizer HUD
	class UFSMVisualizerHUD* FSMVisualizer{};

	UAnimationAsset* HitAnimation{};

	FTimerHandle UpdateInfoTimerHandle;
};
