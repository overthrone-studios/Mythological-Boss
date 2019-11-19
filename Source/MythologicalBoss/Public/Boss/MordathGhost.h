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

	// Pause current animation, triggers a reset timer when called
	void PauseAnimsWithTimer();

	void Die() override;

protected:
	void BeginPlay() override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	#pragma region Follow
		void OnEnterFollowState() override;
		void UpdateFollowState(float Uptime, int32 Frames) override;
		void OnExitFollowState() override;
	#pragma endregion 

	#pragma region Think
		void OnEnterThinkState() override;
		void UpdateThinkState(float Uptime, int32 Frames) override;
		void OnExitThinkState() override;
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

	void StopActionMontage() override;

	void ChooseAction() override;

	void ExecuteAction(class UMordathActionData* ActionData) override;

	// The data the boss will reference during it's life
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

	FName CurrentMontageSection = "None";

	uint8 MoveDirection = 0;

	UMaterialInterface* GhostMaterial;

	class UAnimMontage* CurrentLongAttackMontage;
};
