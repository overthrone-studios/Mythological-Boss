// Copyright Overthrone Studios 2019

#pragma once

#include "MordathBase.h"
#include "Combat/ComboData.h"
#include "OverthroneEnums.h"
#include "Mordath.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Mordath : public FCharacterDebug
{
	GENERATED_BODY()

	// Log the delay time (in seconds) when a new combo is chosen to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogComboDelayTime : 1;

	// Log the current combo name to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCurrentCombo : 1;

	// Log the current stage combo to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogCurrentStageCombo : 1;

	// Log the hit count to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogHits : 1;

	// Log the think time to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogThinkTime : 1;
	
	// Log the retreat time to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogRetreatTime : 1;
};

UCLASS()
class MYTHOLOGICALBOSS_API AMordath final : public AMordathBase
{
	GENERATED_BODY()

public:
	AMordath();

	void AddDebugMessages() override;

	// Returns true if we are in the first stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInFirstStage() const;

	// Returns true if we are in the second stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInSecondStage() const;

	// Returns true if we are in the third stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInThirdStage() const;

	// Returns true if we are in the recovering state/animation
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsRecovering() const;

	// Returns true if we are currently dashing
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDashing() const;

	// Returns true if we are currently strafing left or right
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsStrafing() const;

	// Returns true if we are damaged by the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDamaged() const;

	// Returns true if we are currently stunned by an attack
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsStunned() const;

	// Returns true if we are kicking the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsKicking() const;

	// Returns true if we are performing a super close range action
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsPerformingCloseAction() const;

	// Returns true if we are transitioning to the next stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsTransitioning() const;

	// Returns true if we are tired
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsTired() const;

	// Returns true if we are currently doing a back hand attack
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsDoingBackHand() const;

	// Returns true if we are teleporting to another location
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsTeleporting() const;

	// Returns true if we are executing a far range action
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsPerformingFarAction() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void EnterStage(EBossStage_Mordath InStage);

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void SpawnLightningStrike(const FVector& LocationToSpawn, const FRotator& Rotation = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void SpawnGhost();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void SpawnGhostDelayed(int32 Amount, float DelayInterval = 1.0f);

	void ToggleLockSelf() override;

	void Die() override;

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

	// List of debugging options
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		FDebug_Mordath Debug;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;
	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void OnLowHealth() override;

	void BeginTakeDamage(float DamageAmount) override;
	void ApplyDamage(float DamageAmount, const FDamageEvent& DamageEvent) override;
	void EndTakeDamage() override;

	void StopActionMontage() override;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void UpdateDamageValueInMainHUD(float DamageAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void DestroySelf();

	void ChooseCombo() override;
	float ChooseComboDelayed() override;

	void ChooseAction() override;

	void ExecuteAction(class UMordathActionData* ActionData) override;

	bool CanAttack() const override;

	#pragma region Events
	// Called when the player's health is less than or equal to 0
	void OnPlayerDeath() override;

	UFUNCTION()
		void OnAttackParryed();

	UFUNCTION()
		void OnAttackBlocked();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void OnFirstStageHealth();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void OnSecondStageHealth();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void OnThirdStageHealth();

	UFUNCTION(BlueprintImplementableEvent)
		void BeginFirstStage();

	UFUNCTION(BlueprintImplementableEvent)
		void BeginSecondStage();

	UFUNCTION(BlueprintImplementableEvent)
		void BeginThirdStage();

	UFUNCTION(BlueprintImplementableEvent, Category = "Mordath")
		void OnAfterTakeDamage();

	UFUNCTION()
		void OnDisappeared();

	UFUNCTION()
		void OnReappeared();

	UFUNCTION()
		void OnEnterPerfectDashWindow();
	#pragma endregion 

	#pragma region Any States
		#pragma region Main FSM
			void OnEnterAnyState(int32 ID, FName Name) override;
			void UpdateAnyState(int32 ID, FName Name, float Uptime, int32 Frames) override;
			void OnExitAnyState(int32 ID, FName Name) override;
		#pragma endregion

		#pragma region Range FSM
			void OnEnterAnyRangeState(int32 ID, FName Name) override;
			void UpdateAnyRangeState(int32 ID, FName Name, float Uptime, int32 Frames) override;
			void OnExitAnyRangeState(int32 ID, FName Name) override;
		#pragma endregion

		#pragma region Stage FSM
		UFUNCTION()
			void OnEnterAnyStageState(int32 ID, FName Name);
		UFUNCTION()
			void UpdateAnyStageState(int32 ID, FName Name, float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitAnyStageState(int32 ID, FName Name);
		#pragma endregion
	#pragma endregion 

	#pragma region Boss States
		#pragma region Follow
			void OnEnterFollowState() override;
			void UpdateFollowState(float Uptime, int32 Frames) override;
			void OnExitFollowState() override;
		#pragma endregion 

		#pragma region Think
			void OnEnterThinkState() override;
			void UpdateThinkState(float Uptime, int32 Frames) override;
			void OnExitThinkState() override;
		#pragma endregion 

		#pragma region Action
			void OnEnterActionState() override;
			void UpdateActionState(float Uptime, int32 Frames) override;
			void OnExitActionState() override;
		#pragma endregion 

		#pragma region Close Action
		UFUNCTION()
			void OnEnterCloseActionState();
		UFUNCTION()
			void UpdateCloseActionState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitCloseActionState();
		#pragma endregion 

		#pragma	region Close Action
		UFUNCTION()
			void OnEnterFarActionState();
		UFUNCTION()
			void UpdateFarActionState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitFarActionState();
		#pragma endregion 

		#pragma region Damaged
		UFUNCTION()
			void OnEnterDamagedState();
		UFUNCTION()
			void UpdateDamagedState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDamagedState();
		#pragma endregion 

		#pragma region Death
			void OnEnterDeathState() override;
			void UpdateDeathState(float Uptime, int32 Frames) override;
			void OnExitDeathState() override;
		#pragma endregion 

		#pragma region Transition
		UFUNCTION()
			void OnEnterTransitionState();
		UFUNCTION()
			void UpdateTransitionState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitTransitionState();
		#pragma endregion 

		#pragma region Stunned
		UFUNCTION()
			void OnEnterStunnedState();
		UFUNCTION()
			void UpdateStunnedState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitStunnedState();
		#pragma endregion 

		#pragma region Laugh
		UFUNCTION()
			void OnEnterLaughState();
		UFUNCTION()
			void UpdateLaughState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitLaughState();
		#pragma endregion 

		#pragma region Dash
		UFUNCTION()
			void OnEnterDashState();
		UFUNCTION()
			void UpdateDashState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDashState();
		#pragma endregion 

		#pragma region Dash Combat
		UFUNCTION()
			void OnEnterDashCombatState();
		UFUNCTION()
			void UpdateDashCombatState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDashCombatState();
		#pragma endregion 

		#pragma region Tired
		UFUNCTION()
			void OnEnterTiredState();
		UFUNCTION()
			void UpdateTiredState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitTiredState();
		#pragma endregion 

		#pragma region Back Hand
		UFUNCTION()
			void OnEnterBackHandState();
		UFUNCTION()
			void UpdateBackHandState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitBackHandState();
		#pragma endregion 

		#pragma region Teleport
		UFUNCTION()
			void OnEnterTeleportState();
		UFUNCTION()
			void UpdateTeleportState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitTeleportState();
		#pragma endregion 

		#pragma region Retreat
		UFUNCTION()
			void OnEnterRetreatState();
		UFUNCTION()
			void UpdateRetreatState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitRetreatState();
		#pragma endregion 

	#pragma region Kick
		UFUNCTION()
			void OnEnterKickState();
		UFUNCTION()
			void UpdateKickState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitKickState();
	#pragma endregion 

	#pragma region Recover
		UFUNCTION()
			void OnEnterRecoverState();
		UFUNCTION()
			void UpdateRecoverState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitRecoverState();
	#pragma endregion 

	#pragma region Locked
		void OnEnterLockedState() override;
		void UpdateLockedState(float Uptime, int32 Frames) override;
		void OnExitLockedState() override;
	#pragma endregion 
	#pragma endregion

	#pragma region Range States
		#pragma region Close
			void OnEnterCloseRange() override;
			void UpdateCloseRange(float Uptime, int32 Frames) override;
			void OnExitCloseRange() override;
		#pragma endregion 

		#pragma region Mid
			void OnEnterMidRange() override;
			void UpdateMidRange(float Uptime, int32 Frames) override;
			void OnExitMidRange() override;
		#pragma endregion 

		#pragma region Far
			void OnEnterFarRange() override;
			void UpdateFarRange(float Uptime, int32 Frames) override;
			void OnExitFarRange() override;

			void OnEnterSuperCloseRange() override;
			void UpdateSuperCloseRange(float Uptime, int32 Frames) override;
			void OnExitSuperCloseRange() override;
		#pragma endregion 
	#pragma endregion

	#pragma region Boss Stages

	#pragma region Stage 1
	UFUNCTION()
		void OnEnterFirstStage();
	UFUNCTION()
		void UpdateFirstStage(float Uptime, int32 Frames);
	UFUNCTION()
		void OnExitFirstStage();
	#pragma endregion 

	#pragma region Stage 2
	UFUNCTION()
		void OnEnterSecondStage();
	UFUNCTION()
		void UpdateSecondStage(float Uptime, int32 Frames);
	UFUNCTION()
		void OnExitSecondStage();
	#pragma endregion 

	#pragma region Stage 3
	UFUNCTION()
		void OnEnterThirdStage();
	UFUNCTION()
		void UpdateThirdStage(float Uptime, int32 Frames);
	UFUNCTION()
		void OnExitThirdStage();
	#pragma endregion 
	#pragma endregion 

	#pragma region Components
	// The boss's stage Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UFSM* StageFSM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UTeleportationComponent* TeleportationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UDashComponent* DashComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UAttackIndicatorComponent* FlashIndicator;
	#pragma endregion

	// The health value where we enter the second stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float SecondStageHealth = 0.6f;

	// The health value where we enter the third stage
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float ThirdStageHealth = 0.3f;

	UPROPERTY(EditInstanceOnly, Category = "Mordath Combat", DisplayName = "Stage 2 Transition Anim")
		class UAnimMontage* Stage2_Transition;

	UPROPERTY(EditInstanceOnly, Category = "Mordath Combat", DisplayName = "Stage 3 Transition Anim")
		class UAnimMontage* Stage3_Transition;

	// The actor to spawn when transitioning to the next stage
	UPROPERTY(EditInstanceOnly, Category = "Mordath Combat", DisplayName = "Lightning Strike Actor")
		TSubclassOf<AActor> LightningStrikeClass;

	// The actor to spawn when transitioning to the next stage
	UPROPERTY(EditInstanceOnly, Category = "Mordath Combat", DisplayName = "Ghost Actor")
		TSubclassOf<AActor> GhostClass;
		
	// The bone name of Mordath's skeleton where the lock-on indiciator should be placed on
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Combat")
		FName LockOnBoneName = "spine01_jnt";

	// The color to use when displaying the floating damage text widget
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Combat")
		TMap<int32, FLinearColor> DamageValueColor;

	// The data the boss will reference during stage 1 of the fight
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		class UMordathStageData* StageOneData;

	// The data the boss will reference during stage 2 of the fight
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		class UMordathStageData* StageTwoData;

	// The data the boss will reference during stage 3 of the fight
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		class UMordathStageData* StageThreeData;

	// How long (in seconds) should we stay dead before being destroyed?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath", meta = (ClampMin = 0.0f))
		float DeathTime = 2.0f;

private:
	//TSubclassOf<class APotionBase> HealthPotion;

	float ThinkTime = 0.0f;
	float RetreatTime = 0.0f;

	FString CurrentMontageName = "None";

	class UMordathActionData* SuperCloseRange_ActionData;
	class UMordathActionData* FarRange_ActionData;

	class UAnimMontage* PreviousActionMontage;

	FTimerHandle TH_SpawnGhostDelay;
};
