// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneCharacter.h"
#include "Combat/ComboData.h"
#include "OverthroneEnums.h"
#include "Mordath.generated.h"

USTRUCT(BlueprintType)
struct FDebug_Mordath : public FCharacterDebug
{
	GENERATED_BODY()

	// Log the distance between this and the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDistance : 1;

	// Log the direction from this to the player to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bLogDirection : 1;

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
class MYTHOLOGICALBOSS_API AMordath final : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AMordath();

	void AddDebugMessages() override;

	// Returns the current action damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetActionDamage() const;

	// Returns the light attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetShortAttackDamage() const;

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetLongAttackDamage() const;

	// Returns the special attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetSpecialAttackDamage() const;

	// Returns the attack radius value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetAttackRadius() const;

	// Returns the recent damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetRecentDamage() const;

	bool IsAttacking() const override;

	// Returns true if we are light attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsShortAttacking() const;

	// Returns true if we are heavy attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsLongAttacking() const;

	// Returns true if we are special attacking
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsSpecialAttacking() const;

	// Returns true if we are in the first stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInFirstStage() const;

	// Returns true if we are in the second stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInSecondStage() const;

	// Returns true if we are in the third stage
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsInThirdStage() const;
	
	// Returns true if we are super close to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsSuperCloseRange() const;

	// Returns true if we are in close distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsCloseRange() const;

	// Returns true if we are in mid distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsMidRange() const;

	// Returns true if we are in far distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsFarRange() const;

	// Returns true if we are in a state that is immune to damage
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool InInvincibleState() const;

	// Returns true if we the combo delay timer is active
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsWaitingForNewCombo() const;

	// Returns true if we want to move towards our right vector
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool WantsMoveRight() const;

	// Returns true if we are in the recovering state/animation
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsRecovering() const;

	// Returns true if we are delaying our current attack
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDelayingAction() const;

	// Returns true if we are currently dashing
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDashing() const;

	// Returns true if we are currently strafing left or right
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsStrafing() const;

	// Returns true if we are damaged by the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDamaged() const;

	// Returns true if we are currently thinking
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsThinking() const;

	// Returns true if we are currently stunned by an attack
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsStunned() const;

	// Returns true if we are kicking the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsKicking() const;

	// Returns true if we have finished playing our current action montage
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool HasFinishedAction() const;

	// Returns true if we have finished playing our current action montage
	bool HasFinishedAction(class UAnimMontage* ActionMontage) const;

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

	// Returns true if the time to execute has been reached
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool IsExecutionTimeExpired() const;

	// Returns the movement speed based on the current range/distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		float GetMovementSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void EnterStage(EBossStage_Mordath InStage);

	UFUNCTION(BlueprintPure, Category = "Mordath")
		class UForceFeedbackEffect* GetCurrentForceFeedbackEffect() const;

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

	// List of debugging options
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath")
		FDebug_Mordath Debug;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ChangeHitboxSize(float NewRadius) override;
	void UpdateCharacterInfo() override;

	void BroadcastLowHealth() override;
	void OnLowHealth() override;

	void SendInfo();

	void FinishStun();

	void BeginTakeDamage(float DamageAmount) override;
	void ApplyDamage(float DamageAmount) override;
	void EndTakeDamage() override;

	void OnExecutionTimeExpired();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void FacePlayer(float RotationSpeed = 10.0f);

	void FacePlayer_Instant();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void FacePlayerBasedOnActionData(const class UMordathActionData* ActionData);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseComboWithDelay();
	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseCombo();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void NextAction();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void UpdateDamageValueInMainHUD(float DamageAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ExecuteAction(class UMordathActionData* ActionData);

	void Die() override;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Misc")
		void DestroySelf();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void PlayActionMontage();

	void PlayActionMontage(class UMordathActionData* ActionData);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void StopActionMontage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseAction();

	UFUNCTION(BlueprintPure, Category = "Mordath | Misc")
		float GetDistanceToPlayer() const;

	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		FVector GetDirectionToPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void ChooseMovementDirection();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void EncirclePlayer();

	UFUNCTION(BlueprintCallable, Category = "Mordath")
		void ResetMeshScale();

	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		bool CanAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ResetActionDamage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void IncreaseAttackDamage(const float& Multiplier);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void StopMoving();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void StartExecutionExpiryTimer();

	#pragma region Events
	// Called when the player's health is less than or equal to 0
	UFUNCTION()
		void OnPlayerDeath();

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Mordath")
		void OnAfterTakeDamage();
	#pragma endregion 

	UFUNCTION()
		void OnEnterAnyState();
	UFUNCTION()
		void UpdateAnyState();
	UFUNCTION()
		void OnExitAnyState();

	UFUNCTION()
		void OnEnterAnyRangeState();
	UFUNCTION()
		void UpdateAnyRangeState();
	UFUNCTION()
		void OnExitAnyRangeState();

	UFUNCTION()
		void OnEnterAnyStageState();
	UFUNCTION()
		void UpdateAnyStageState();
	UFUNCTION()
		void OnExitAnyStageState();

	#pragma region Boss States
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

		#pragma region Think
		UFUNCTION()
			void OnEnterThinkState();
		UFUNCTION()
			void UpdateThinkState();
		UFUNCTION()
			void OnExitThinkState();
		#pragma endregion 

		#pragma region Action
		UFUNCTION()
			void OnEnterActionState();
		UFUNCTION()
			void UpdateActionState();
		UFUNCTION()
			void OnExitActionState();
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

		#pragma region Dash
		UFUNCTION()
			void OnEnterDashState();
		UFUNCTION()
			void UpdateDashState();
		UFUNCTION()
			void OnExitDashState();
		#pragma endregion 

		#pragma region Dash Combat
		UFUNCTION()
			void OnEnterDashCombatState();
		UFUNCTION()
			void UpdateDashCombatState();
		UFUNCTION()
			void OnExitDashCombatState();
		#pragma endregion 

		#pragma region Strafe
		UFUNCTION()
			void OnEnterStrafeState();
		UFUNCTION()
			void UpdateStrafeState();
		UFUNCTION()
			void OnExitStrafeState();
		#pragma endregion 

		#pragma region Tired
		UFUNCTION()
			void OnEnterTiredState();
		UFUNCTION()
			void UpdateTiredState();
		UFUNCTION()
			void OnExitTiredState();
		#pragma endregion 

		#pragma region Back Hand
		UFUNCTION()
			void OnEnterBackHandState();
		UFUNCTION()
			void UpdateBackHandState();
		UFUNCTION()
			void OnExitBackHandState();
		#pragma endregion 

		#pragma region Teleport
		UFUNCTION()
			void OnEnterTeleportState();
		UFUNCTION()
			void UpdateTeleportState();
		UFUNCTION()
			void OnExitTeleportState();
		#pragma endregion 

		#pragma region Retreat
		UFUNCTION()
			void OnEnterRetreatState();
		UFUNCTION()
			void UpdateRetreatState();
		UFUNCTION()
			void OnExitRetreatState();
		#pragma endregion 

	#pragma region Kick
		UFUNCTION()
			void OnEnterKickState();
		UFUNCTION()
			void UpdateKickState();
		UFUNCTION()
			void OnExitKickState();
	#pragma endregion 

	#pragma region Recover
		UFUNCTION()
			void OnEnterRecoverState();
		UFUNCTION()
			void UpdateRecoverState();
		UFUNCTION()
			void OnExitRecoverState();
	#pragma endregion 
	#pragma endregion

	#pragma region Range States
		#pragma region Close
		UFUNCTION()
			void OnEnterCloseRange();
		UFUNCTION()
			void UpdateCloseRange();
		UFUNCTION()
			void OnExitCloseRange();
		#pragma endregion 

		#pragma region Mid
		UFUNCTION()
			void OnEnterMidRange();
		UFUNCTION()
			void UpdateMidRange();
		UFUNCTION()
			void OnExitMidRange();
		#pragma endregion 

		#pragma region Far
		UFUNCTION()
			void OnEnterFarRange();
		UFUNCTION()
			void UpdateFarRange();
		UFUNCTION()
			void OnExitFarRange();

		UFUNCTION()
			void OnEnterSuperCloseRange();
		UFUNCTION()
			void UpdateSuperCloseRange();
		UFUNCTION()
			void OnExitSuperCloseRange();
		#pragma endregion 
	#pragma endregion

	#pragma region Boss Stages

	#pragma region Stage 1
	UFUNCTION()
		void OnEnterFirstStage();
	UFUNCTION()
		void UpdateFirstStage();
	UFUNCTION()
		void OnExitFirstStage();
	#pragma endregion 

	#pragma region Stage 2
	UFUNCTION()
		void OnEnterSecondStage();
	UFUNCTION()
		void UpdateSecondStage();
	UFUNCTION()
		void OnExitSecondStage();
	#pragma endregion 

	#pragma region Stage 3
	UFUNCTION()
		void OnEnterThirdStage();
	UFUNCTION()
		void UpdateThirdStage();
	UFUNCTION()
		void OnExitThirdStage();
	#pragma endregion 
	#pragma endregion 

	#pragma region Components
	// The boss's range Finite State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mordath")
		class UFSM* RangeFSM;

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

	// Our custom AI controller
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | AI")
		class ABossAIController* BossAIController{};
	
	// Cached anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Animation")
		class UMordathAnimInstance* MordathAnimInstance{};

	// The combo we are using
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		UComboData* ChosenCombo;

	// Used to iterate, select or remove a combo, this to avoid touching the actual combos list
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		TArray<UComboData*> CachedCombos;

private:
	//TSubclassOf<class APotionBase> HealthPotion;

	void MoveForward(float Scale = 1.0f);
	void MoveRight(float Scale = 1.0f);

	float ActionDamage = 0.0f;

	float ThinkTime = 0.0f;
	float RetreatTime = 0.0f;

	float DistanceToPlayer = 0.0f;
	FVector DirectionToPlayer;

	FComboData_Action* CurrentActionData;

	FName CurrentMontageSection = "None";

	class UAnimMontage* CurrentLongAttackMontage;
	class UMordathActionData* SuperCloseRange_ActionData;

	class UAnimMontage* PreviousActionMontage;

	class UMordathStageData* CurrentStageData;

	class AOverthroneCharacter* PlayerCharacter;

	// Timer handles
	FTimerHandle TH_UpdateInfo;

	FTimerHandle TH_ComboDelay;
};
