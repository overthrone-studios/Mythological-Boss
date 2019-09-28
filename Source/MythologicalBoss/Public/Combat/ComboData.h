// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MordathActionData.h"
#include "TimerManager.h"
#include "ComboData.generated.h"

USTRUCT(BlueprintType)
struct FComboData_Action
{
	GENERATED_BODY()

	// The action to execute
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		UMordathActionData* Action;

	// Can we teleport before we do this action?
	UPROPERTY(EditInstanceOnly)
		uint8 bCanTeleportWithAction : 1;

	// The range that this action can be executed at
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EBossRange_Mordath> RangeToExecute = Close;
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
		void NextAction();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		bool IsAtLastAction();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		bool IsDelayEnabled();
	
	UFUNCTION(BlueprintPure, Category = "Combo Data")
		float GetActionDelayTime();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		float GetDeviation();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		FTimerHandle& GetActionDelayTimer();

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		int32 GetCurrentActionIndex() const { return ActionIndex; }

	UFUNCTION(BlueprintPure, Category = "Combo Data")
		FComboData_Action& GetCurrentActionData() { return CurrentAction; }

protected:
	// Should we wait between actions?
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		uint8 bDelayBetweenActions : 1;

	// The amount of time (in seconds) we should wait
	UPROPERTY(EditInstanceOnly, Category = "Combos", meta = (EditCondition = "bDelayBetweenActions", ClampMin = 0.0f))
		float Delay = 1.0f;

	// Adds a random range to Delay
	UPROPERTY(EditInstanceOnly, Category = "Combos", meta = (EditCondition = "bDelayBetweenActions", ClampMin = 0.0f))
		float RandomDeviation = 0.0f;

	// The list of actions to go through sequentially
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<FComboData_Action> Actions;

	FComboData_Action CurrentAction;

private:
	int32 ActionIndex = 0;

	FTimerHandle TH_ActionDelay;
};
