// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "OverthroneCharacter.h"
#include "ComboData.h"
#include "MordathBase.generated.h"

UCLASS(Abstract)
class MYTHOLOGICALBOSS_API AMordathBase : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AMordathBase();

	UFUNCTION(BlueprintPure, Category = "Mordath | General")
		float GetDistanceToPlayer() const;

	UFUNCTION(BlueprintPure, Category = "Mordath | General")
		FVector GetDirectionToPlayer() const;

	// Returns the current action damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetActionDamage() const;

	// Returns the attack radius value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetAttackRadius() const;

	// Returns the recent damage value
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		float GetRecentDamage() const;

	bool IsAttacking() const override;

	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		virtual bool IsShortAttacking() const;
	
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		virtual bool IsLongAttacking() const;
	
	UFUNCTION(BlueprintPure, Category = "Mordath | Combat")
		virtual bool IsSpecialAttacking() const;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;

	void Die() override;

	UFUNCTION(BlueprintCallable, Category = "Mordath | General")
		void FacePlayer(float RotationSpeed = 10.0f);

	UFUNCTION(BlueprintCallable, Category = "Mordath | General")
		void FacePlayer_Instant();

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void MoveForward(float Scale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void MoveRight(float Scale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Movement")
		void ChooseMovementDirection();

	UFUNCTION(BlueprintPure, Category = "Mordath | Movement")
		float GetMovementSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		void PlayActionMontage();

	void PlayActionMontage(class UMordathActionData* ActionData);

	UFUNCTION(BlueprintCallable, Category = "Mordath | Combat")
		virtual void StopActionMontage();

	// Called when the player's health is less than or equal to 0
	UFUNCTION()
		virtual void OnPlayerDeath();

	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		class UAnimMontage* CurrentActionMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Combat")
		class UMordathStageData* CurrentStageData;

	FComboData_Action* CurrentActionData;

	UPROPERTY(BlueprintReadOnly, Category = "Mordath | General")
		class AOverthroneCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Mordath | General")
		float DistanceToPlayer = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Mordath | General")
		FVector DirectionToPlayer = FVector(0.0f);

	// Our custom AI controller
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | AI")
		class ABossAIController* BossAIController{};
	
	// Cached anim instance, to control and trigger animations
	UPROPERTY(BlueprintReadOnly, Category = "Mordath | Animation")
		class UMordathAnimInstance* MordathAnimInstance{};

	float ActionDamage = 0.0f;

	uint8 MovementDirection : 1;
};
