// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.h"
#include "ComboData.generated.h"


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

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		bool IsAtLastAttack();

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		int32 GetCurrentAttackIndex() const { return AttackIndex; }

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		UAttackData* GetCurrentAttackInfo() const { return CurrentAttack; }

protected:
	// Should we wait between attacks?
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		uint8 bDelayBetweenAttacks : 1;

	// The amount of time (in seconds) we should wait
	UPROPERTY(EditInstanceOnly, Category = "Combos", meta = (EditCondition = "bDelayBetweenAttacks", ClampMin = 0.0f, ClampMax = 100.0f))
		float Delay = 1.0f;

	// The list of attacks to go through sequentially
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<UAttackData*> Attacks;

	UAttackData* CurrentAttack;

private:
	int32 AttackIndex = 0;
};
