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

	// The amount of charge we gain after a successful light attack hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0.0f))
		float ChargeGainPerLightHit = 15.0f;

	// The amount of charge we gain after a successful heavy attack hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0.0f))
		float ChargeGainPerHeavyHit = 20.0f;

	// The amount of charge we lose after we've taken damage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0.0f))
		float ChargeLossPerHit = 15.0f;

	// The amount of health we gain when we've successfully landed a charge attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0.0f))
		float HealthGainOnChargeAttack = 80.0f;

	// The maximum amount of time (in seconds) we can hold the charge attack for
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0.0f))
		float MaxChargeHoldTime = 5.0f;

	// The amount of frames to hold the charge for, before releasing it
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (ClampMin = 0))
		int32 ChargeHoldFrames = 25;

	// Should we reset the charge meter when we've taken damage?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack")
		uint8 bResetChargeAfterMaxHits : 1;

	// The number of hits we can take before our charge is reset
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (EditCondition = "bResetChargeAfterMaxHits"))
		uint8 MaxHitsForChargeReset = 3;

	// Should we lose charge overtime?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack")
		uint8 bLoseChargeOvertime : 1;

	// The amount of time (in seconds) to delay, before starting to lose charge
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f, ClampMax = 60.0f))
		float DelayBeforeChargeLoss = 3.0f;

	// The speed of the charge loss (How fast or slow do we lose charge?)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ylva Difficulty Data | Charge Attack", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f))
		float ChargeLossRate = 30.0f;
};
