// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComboComponent.generated.h"

UENUM()
enum EAttackType
{
	Light,
	Heavy,
	Special,
	None
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UAttackComboComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComboComponent();

	// Advances to the next attack in the combo tree
	UFUNCTION(BlueprintCallable, Category = "Attack Combo")
		uint8 AdvanceCombo(enum EAttackType InAttackType);

	// Returns the array of light attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetLightAttacks() const { return LightAttacks; }

	// Returns the array of heavy attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetHeavyAttacks() const { return HeavyAttacks; }

	// Returns the array of special attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetSpecialAttacks() const { return SpecialAttacks; }

	// Returns the current of light attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentLightAttackAnim() const { return LightAttacks[LightAttackIndex-1]; }

	// Returns the current of heavy attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentHeavyAttackAnim() const { return HeavyAttacks[HeavyAttackIndex-1]; }

	// Returns the current of special attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentSpecialAttackAnim() const { return SpecialAttacks[SpecialAttackIndex-1]; }

protected:
	void BeginPlay() override;

	// Resets the combo tree
	void ResetCombo();

	// The list how light attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		TArray<class UAnimMontage*> LightAttacks;

	// The list how heavy attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		TArray<class UAnimMontage*> HeavyAttacks;

	// The list how special attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		TArray<class UAnimMontage*> SpecialAttacks;

	// How deep can the combo tree go?
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 1))
		uint8 ComboTreeDepth = 1;

	// How long (in seconds) should we wait until we reset the combo?
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float ComboResetTime = 1.0f;

	// Log combo tree status
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bDebug : 1;

private:
	uint8 AdvanceCombo_Internal(enum EAttackType InAttackType);

	// Tracks the attacks we've chained
	TArray<TEnumAsByte<EAttackType>> Combo;

	uint8 LightAttackIndex = 0;
	uint8 HeavyAttackIndex = 0;
	uint8 SpecialAttackIndex = 0;

	uint8 TreeIndex = 0;

	FTimerHandle ComboResetTimerHandle;

	// The actor that owns this component
	AActor* Owner;
};
