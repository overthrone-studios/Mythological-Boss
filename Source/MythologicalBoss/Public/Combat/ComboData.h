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
	UPROPERTY(EditInstanceOnly)
		UMordathActionData* Action;

	// The action to execute when ExecutionTime has expired (If this is null/empty, just execute the current action)
	UPROPERTY(EditInstanceOnly)
		UMordathActionData* FailSafeAction;

	// The range that this action can be executed at
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EBossRange_Mordath> RangeToExecute = BRM_Close;

	// The maximum amount of time left (in seconds) to execute this action
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float ExecutionTime = 5.0f;

	// The amount of times to loop this action before continuing to the next action, this will not loop fail safe action. (1 = Execute once, 2 = Execute twice, etc.)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 1))
		uint8 Loops = 1;

	// The time in the animation to 'cut' or stop playing (-1.0 = Do not cut, 0.0 = Stop at the beginning)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = "-1.0"))
		float StopAtTime = -1.0f;

	// Is this a teleport action?
	UPROPERTY(EditInstanceOnly)
		uint8 bIsTeleport : 1;

	// The range mordath will teleport to. If the teleported location is outside the play area, this value will be discarded
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bIsTeleport"))
		TEnumAsByte<EBossRange_Mordath> TeleportToRange = BRM_Mid;

	UPROPERTY(EditInstanceOnly)
		uint8 bSpawnGhost : 1;
	
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bSpawnGhost", ClampMin = 1))
		uint8 GhostCount = 1;

	uint8 ExecutionCount = 0;

	uint8 bExecutionTimeExpired : 1;

	FTimerHandle TH_ExecutionExpiry;
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
		bool WantsToExecuteNonStop();
	
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

	// Should we execute each action in the list non-stop?
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		uint8 bNonStop : 1;

	// The list of actions to go through sequentially
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<FComboData_Action> Actions;

	FComboData_Action CurrentAction;

private:
	int32 ActionIndex = 0;

	FTimerHandle TH_ActionDelay;
};
