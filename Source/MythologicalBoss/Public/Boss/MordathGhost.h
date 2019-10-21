// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "MordathBase.h"
#include "ComboData.h"
#include "MordathGhost.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API AMordathGhost final : public AMordathBase
{
	GENERATED_BODY()
	
public:
	AMordathGhost();

	// Returns the light attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetShortAttackDamage() const;

	// Returns the heavy attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetLongAttackDamage() const;

	// Returns the special attack damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetSpecialAttackDamage() const;

	float GetAttackRadius() const override;

	// Returns true if we are in far distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsFarRange() const;

	// Returns true if we have finished playing our current attack montage
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool HasFinishedAttack() const;

	bool IsShortAttacking() const override;

	bool IsLongAttacking() const override;

	bool IsSpecialAttacking() const override;

	// Returns the movement speed based on the current range/distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		float GetMovementSpeed() const override;

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void FacePlayer();

	// todo remove functions
	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void PlayAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void StopAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseAttack();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void EncirclePlayer();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseCombo();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void NextAttack();

	// Returns true if we are delaying our current attack
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool IsDelayingAttack() const;

	#pragma region Follow
		UFUNCTION()
			void OnEnterFollowState();
		UFUNCTION()
			void UpdateFollowState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitFollowState();
	#pragma endregion 

	#pragma region Think
		UFUNCTION()
			void OnEnterThinkState();
		UFUNCTION()
			void UpdateThinkState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitThinkState();
	#pragma endregion 

	#pragma region Light Attack 1
		UFUNCTION()
			void OnEnterShortAttack1State();
		UFUNCTION()
			void UpdateShortAttack1State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitShortAttack1State();
	#pragma endregion 

	#pragma region Light Attack 2
		UFUNCTION()
			void OnEnterShortAttack2State();
		UFUNCTION()
			void UpdateShortAttack2State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitShortAttack2State();
	#pragma endregion 

	#pragma region Light Attack 3
		UFUNCTION()
			void OnEnterShortAttack3State();
		UFUNCTION()
			void UpdateShortAttack3State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitShortAttack3State();
	#pragma endregion 

	#pragma region Heavy Attack 1
		UFUNCTION()
			void OnEnterLongAttack1State();
		UFUNCTION()
			void UpdateLongAttack1State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitLongAttack1State();
	#pragma endregion 

	#pragma region Heavy Attack 2
		UFUNCTION()
			void OnEnterLongAttack2State();
		UFUNCTION()
			void UpdateLongAttack2State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitLongAttack2State();
	#pragma endregion 

	#pragma region Heavy Attack 3
		UFUNCTION()
			void OnEnterLongAttack3State();
		UFUNCTION()
			void UpdateLongAttack3State(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitLongAttack3State();
	#pragma endregion 

	#pragma region Death
		UFUNCTION()
			void OnEnterDeathState();
		UFUNCTION()
			void UpdateDeathState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitDeathState();
	#pragma endregion 

	// The data the boss will reference during stage 1 of the fight
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mordath")
		class UMordathStageData* StageData;

	int8 ComboIndex = 0; // This is used to choose a random index in the combos list

	// The combo we are using
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		UComboData* ChosenCombo;

	// Used to iterate, select or remove a combo, this to avoid touching the actual combos list
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		TArray<UComboData*> CachedCombos;

private:
	float DefaultRotationSpeed = 10.0f;

	float ShortAttackDamage = 0.0f;
	float LongAttackDamage = 0.0f;
	float SpecialAttackDamage = 0.0f;

	float ThinkTime = 0.0f;
	float RetreatTime = 0.0f;

	float DistanceToPlayer = 0.0f;
	FVector DirectionToPlayer;

	FComboData_Action* CurrentAttackData;

	FName CurrentMontageSection = "None";

	uint8 MoveDirection = 0;

	UMaterialInterface* GhostMaterial;

	class UAnimMontage* CurrentLongAttackMontage;

	class UMordathStageData* CurrentStageData;
};
