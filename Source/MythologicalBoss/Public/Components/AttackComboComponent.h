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

USTRUCT(BlueprintType)
struct FAttacks
{
	GENERATED_BODY()

	// The list of anim montages
	UPROPERTY(EditInstanceOnly)
		TArray<class UAnimMontage*> List;

	// How long (in seconds) should we wait until we can attack again?
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float AttackDelay = 1.0f;
};

USTRUCT(BlueprintType)
struct FAttackChain
{
	GENERATED_BODY()

	// The list of attacks
	UPROPERTY(EditInstanceOnly)
		TArray<TEnumAsByte<EAttackType>> Attacks;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UAttackComboComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComboComponent();

	// Advances to the next attack in the combo tree (-1 means the tree failed to advance)
	UFUNCTION(BlueprintCallable, Category = "Attack Combo")
		int32 AdvanceCombo(enum EAttackType InAttackType);

	// Returns the array of light attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetLightAttacks() const { return LightAttacks.List; }

	// Returns the array of heavy attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetHeavyAttacks() const { return HeavyAttacks.List; }

	// Returns the array of special attacks
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<class UAnimMontage*> GetSpecialAttacks() const { return SpecialAttacks.List; }

	// Returns the current light attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentLightAttackAnim() const { return LightAttacks.List[LightAttackIndex-1]; }

	// Returns the current heavy attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentHeavyAttackAnim() const { return HeavyAttacks.List[HeavyAttackIndex-1]; }

	// Returns the current special attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentSpecialAttackAnim() const { return SpecialAttacks.List[SpecialAttackIndex-1]; }

	// Returns the combo we've created
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<TEnumAsByte<EAttackType>> GetComboChain() const { return Combo; }

protected:
	void BeginPlay() override;

	// Resets the combo tree
	void ResetCombo();

	// The list of light attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		FAttacks LightAttacks;

	// The list of heavy attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		FAttacks HeavyAttacks;

	// The list of special attacks we can use
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		FAttacks SpecialAttacks;

	// The list of attack chains to test against the current attack chain
	UPROPERTY(EditInstanceOnly, Category = "Combo")
		TArray<FAttackChain> AttackChains;

	// How deep can the combo tree go?
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 1))
		uint8 ComboTreeDepth = 1;

	// How long (in seconds) should we wait until we reset the combo? This value must be higher than the attack delays to create combos
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float ComboResetTime = 1.0f;

	// Log the current tree index
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bLogComboIndex : 1;

	// Log the current attack index
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bLogAttackIndex : 1;

	// Log the attack chain history just before we reset
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bLogAttackChain : 1;

	// Log the status of the combo tree (i.e. what it's doing)
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bLogTreeStatus : 1;

private:
	int8 AdvanceCombo_Internal(enum EAttackType InAttackType);
	int8 AdvanceAttack(int8& AttackIndex, const TArray<class UAnimMontage*>& AttackList, const enum EAttackType& InAttackType);

	void DelayAttack(const float& Delay);
	void LogAttackChain(); // Tracks the attacks we've chained

	// The attack chain history
	TArray<TEnumAsByte<EAttackType>> Combo;
	TArray<TEnumAsByte<EAttackType>> PreviousCombo;

	int8 LightAttackIndex = 0;
	int8 HeavyAttackIndex = 0;
	int8 SpecialAttackIndex = 0;

	int8 TreeIndex = 0;

	FTimerHandle AttackDelayTimerHandle;
	FTimerHandle ComboResetTimerHandle;

	// The actor that owns this component
	AActor* Owner;
};
