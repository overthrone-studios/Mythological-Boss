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

	bool IsShortAttacking() const override;

	bool IsLongAttacking() const override;

	bool IsSpecialAttacking() const override;

	// Returns true if we are in far distance to the player
	UFUNCTION(BlueprintPure, Category = "Mordath | Stage")
		bool IsFarRange() const;

	// Returns true if we have finished playing our current attack montage
	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		bool HasFinishedAttack() const;

	// Returns the movement speed based on the current range/distance to the player
	float GetMovementSpeed() const override;

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// todo remove functions
	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void PlayAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void StopAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void ChooseAttack();

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

	#pragma region Action
		UFUNCTION()
			void OnEnterActionState();
		UFUNCTION()
			void UpdateActionState(float Uptime, int32 Frames);
		UFUNCTION()
			void OnExitActionState();
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
};
