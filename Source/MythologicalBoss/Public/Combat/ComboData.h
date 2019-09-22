// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.h"
#include "TimerManager.h"
#include "ComboData.generated.h"

USTRUCT(BlueprintType)
struct FComboData_Attack
{
	GENERATED_BODY()

	// The list of attacks to go through sequentially
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		UAttackData* Attack;

	// Can we teleport before we attack?
	UPROPERTY(EditInstanceOnly)
		uint8 bCanTeleportWithAttack : 1;
};

/**
 *	An asset containing a series of attacks
 */
UCLASS()
class MYTHOLOGICALBOSS_API UComboData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		void Init();

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		void NextAttack();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		bool IsAtLastAttack();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		bool IsDelayEnabled();
	
	UFUNCTION(BlueprintPure, Category = "Combo Data")
		float GetAttackDelayTime();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		float GetDeviation();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		FTimerHandle& GetAttackDelayTimer();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		int32 GetCurrentAttackIndex() const { return AttackIndex; }

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		FComboData_Attack& GetCurrentAttackData() { return CurrentAttack; }

protected:
	// Should we wait between attacks?
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		uint8 bDelayBetweenAttacks : 1;

	// The amount of time (in seconds) we should wait
	UPROPERTY(EditInstanceOnly, Category = "Combos", meta = (EditCondition = "bDelayBetweenAttacks", ClampMin = 0.0f, ClampMax = 100.0f))
		float Delay = 1.0f;

	// Adds a random range to Delay
	UPROPERTY(EditInstanceOnly, Category = "Combos", meta = (EditCondition = "bDelayBetweenAttacks", ClampMin = 0.0f, ClampMax = 100.0f))
		float RandomDeviation = 0.0f;

	// The list of attacks to go through sequentially
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<FComboData_Attack> Attacks;

	FComboData_Attack CurrentAttack;

private:
	int32 AttackIndex = 0;

	FTimerHandle AttackDelayTimerHandle;
};
