// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Boss/MordathBase.h"
#include "MordathTutorial.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopAtTimeReachedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadSignature);

USTRUCT(BlueprintType)
struct FMordathAction_Tutorial
{
	GENERATED_BODY()

	// The data the boss will reference during it's life
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mordath")
		UMordathActionData* ActionData;

	// At what point in the animation do we pause
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mordath")
		float PauseAtTime = 1.0f;

	// Should mordath ghost be killed when the tutorial action has been completed?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mordath")
		uint8 bKillOnActionCompleted : 1;
};

/**
 * Tutorial version of Mordath
 */
UCLASS()
class MYTHOLOGICALBOSS_API AMordathTutorial final : public AMordathBase
{
	GENERATED_BODY()
	
public:
	AMordathTutorial();

	UPROPERTY(BlueprintAssignable, Category = "Mordath Tutorial")
		FOnStopAtTimeReachedSignature OnStopAtTimeReached;

	UPROPERTY(BlueprintAssignable, Category = "Mordath Tutorial")
		FOnDeadSignature OnDead;

	UFUNCTION(BlueprintCallable, Category = "Mordath Tutorial")
		void ResetSelf();

	UFUNCTION(BlueprintPure, Category = "Mordath Tutorial")
		bool CanKillOnActionCompleted() const { return Actions[CurrentActionIndex].bKillOnActionCompleted; }

	UFUNCTION(BlueprintCallable, Category = "Mordath Tutorial")
		void NotifyTutorialCompleted();
		
	float GetMovementSpeed() const override;

	void LockSelf() override;
	void UnlockSelf() override;

protected:
	void BeginPlay() override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool CanAttack() const override;

	#pragma region Follow
		void OnEnterFollowState() override;
		void UpdateFollowState(float Uptime, int32 Frames) override;
		void OnExitFollowState() override;
	#pragma endregion 

	#pragma region Action
		void OnEnterActionState() override;
		void UpdateActionState(float Uptime, int32 Frames) override;
		void OnExitActionState() override;
	#pragma endregion 

	#pragma region Death
		void OnEnterDeathState() override;
		void UpdateDeathState(float Uptime, int32 Frames) override;
		void OnExitDeathState() override;
	#pragma endregion 

	void UpdateLockedState(float Uptime, int32 Frames) override;

	#pragma region Close
		void OnEnterCloseRange() override;
		void UpdateCloseRange(float Uptime, int32 Frames) override;
		void OnExitCloseRange() override;
	#pragma endregion 

	#pragma region Mid
		void OnEnterMidRange() override;
		void UpdateMidRange(float Uptime, int32 Frames) override;
		void OnExitMidRange() override;
	#pragma endregion 

	#pragma region Far
		void OnEnterFarRange() override;
		void UpdateFarRange(float Uptime, int32 Frames) override;
		void OnExitFarRange() override;
	#pragma endregion 

	#pragma region Super Close
		void OnEnterSuperCloseRange() override;
		void UpdateSuperCloseRange(float Uptime, int32 Frames) override;
		void OnExitSuperCloseRange() override;
	#pragma endregion 

	void StopActionMontage() override;

	void ChooseAction() override;

	void ExecuteAction(class UMordathActionData* ActionData) override;

	// The data the boss will reference during it's life
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mordath")
		TArray<FMordathAction_Tutorial> Actions;

private:
	UMaterialInterface* GhostMaterial;

	uint8 CurrentActionIndex = 0;

	uint8 bCurrentTutorialCompleted : 1;
	uint8 bStopAtTimeEventTriggered : 1;
};
