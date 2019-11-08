// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Misc/CharacterDifficultyData.h"
#include "YlvaDifficultyData.generated.h"

/**
 * Ylva's difficulty data
 */
UCLASS()
class MYTHOLOGICALBOSS_API UYlvaDifficultyData final : public UCharacterDifficultyData
{
	GENERATED_BODY()
	
public:
	UYlvaDifficultyData();

	// Ylva default stamina value for this difficulty
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float DefaultStamina = 1000.0f;

	// The rate of stamina regeneration (How fast do we replenish stamina)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float RegenerationRate = 250.0f;

	// The amount of time (in seconds) we should wait before regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float RegenerationDelay = 0.7f;

	// The stamina value to subtract when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float LightAttack = 100.0f;

	// The stamina value to subtract when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float HeavyAttack = 200.0f;

	// The stamina value to subtract when dashing
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float Dash = 150.0f;

	// The stamina value to subtract when dash attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float DashAttack = 200.0f;

	// The stamina value to subtract when charge attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float ChargeAttack = 100.0f;

	// The stamina value to subtract while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float Run = 100.0f;

	// The stamina value to subtract when being hit while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Difficulty Data | Stamina", meta = (ClampMin = 0.0f))
		float ShieldHit = 0.0f;
};
