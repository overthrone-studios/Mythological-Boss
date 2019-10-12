// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OverthroneEnums.h"
#include "AlphaBlend.h"
#include "AttackComboComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboTreeResetSignature);

USTRUCT(BlueprintType)
struct FPlayerMontageSection_Data
{
	GENERATED_BODY()

	// The rate/speed of the animation (0.0 = No rate, 1.0 = Normal, 2.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float PlayRate = 1.0f;

	// The type of blending to use
	UPROPERTY(EditInstanceOnly)
		EAlphaBlendOption BlendOption;
};

USTRUCT(BlueprintType)
struct FPlayerAttack_Data
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditInstanceOnly)
		FPlayerMontageSection_Data Anticipation;

	UPROPERTY(EditInstanceOnly)
		FPlayerMontageSection_Data Contact;

	UPROPERTY(EditInstanceOnly)
		FPlayerMontageSection_Data Recovery;
};

USTRUCT(BlueprintType)
struct FAttacks
{
	GENERATED_BODY()

	// The list of attacks
	UPROPERTY(EditInstanceOnly)
		TArray<FPlayerAttack_Data> List;

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
		TArray<TEnumAsByte<EAttackType_Player>> Attacks;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UAttackComboComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComboComponent();

	FOnComboTreeResetSignature OnComboReset;

	// Advances to the next attack in the combo tree
	FPlayerAttack_Data* AdvanceCombo(EAttackType_Player InAttackType);

	// Clears our current attack to None
	UFUNCTION(BlueprintCallable, Category = "Attack Combo")
		void ClearCurrentAttack();

	// Resets the combo tree
	UFUNCTION(BlueprintCallable, Category = "Attack Combo")
		void ResetCombo();

	// Resets every montages blend out time to their original value
	UFUNCTION(BlueprintCallable, Category = "Attack Combo")
		void ResetAllMontageBlendTimes();

	// Returns the current light attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentLightAttackAnim() const { return LightAttacks.List[LightAttackIndex].AttackMontage; }

	// Returns the current heavy attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentHeavyAttackAnim() const { return HeavyAttacks.List[HeavyAttackIndex].AttackMontage; }

	// Returns the current special attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE class UAnimMontage* GetCurrentSpecialAttackAnim() const { return SpecialAttacks.List[SpecialAttackIndex].AttackMontage; }

	// Returns the current light attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE FPlayerAttack_Data& GetCurrentLightAttackData() { return LightAttacks.List[LightAttackIndex]; }

	// Returns the current heavy attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE FPlayerAttack_Data& GetCurrentHeavyAttackData() { return HeavyAttacks.List[HeavyAttackIndex]; }

	// Returns the current special attack anim montage
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE FPlayerAttack_Data& GetCurrentSpecialAttackData() { return SpecialAttacks.List[SpecialAttackIndex]; }

	// Returns the combo we've created
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE TArray<TEnumAsByte<EAttackType_Player>> GetComboChain() const { return PreviousCombo; }

	// Returns the current attack type we are on
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE EAttackType_Player GetCurrentAttack() const { return CurrentAttack; }

		// Returns the current attack animation montage we are on
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		class UAnimMontage* GetCurrentAttackAnim() const { return CurrentAttackData->AttackMontage; }

	// Returns the current attack animation montage we are on
	FPlayerAttack_Data* GetCurrentAttackData() const { return CurrentAttackData; }

	// Returns the current attack type we are on as a string
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FString GetCurrentAttackAsString() const;

	// Returns the damage multiplier value
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE float GetDamageMultiplier() const { return ComboMultiplier; }

	// Returns the damage multiplier value
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		FORCEINLINE uint8 GetComboTreeDepth() const { return TreeIndex; }

	// Returns true if the delay timer is active
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		bool CanAttack() const;

	// Returns true if the combo reset timer is inactive
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		bool IsWaitingForComboReset() const;

	// Returns true if we are at the end of the tree
	UFUNCTION(BlueprintPure, Category = "Attack Combo")
		bool IsAtTreeEnd() const;

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	// The value to multiply the actor's base damage by (after the first attack) 
	UPROPERTY(EditInstanceOnly, Category = "Combo", meta = (ClampMin = 1.0f))
		float ComboMultiplier = 1.0f;

	// The value to increment the combo multiplier amount by
	UPROPERTY(EditInstanceOnly, Category = "Combo", meta = (ClampMin = 0.0f))
		float MultiplierIncrementAmount = 0.2f;

	// The amount of times we should add the multiplier to the actor's base damage (after the first attack)
	UPROPERTY(EditInstanceOnly, Category = "Combo", meta = (ClampMin = 1))
		uint8 ComboMultiplierCount = 2;

	// Should the tree be infinite?
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 1))
		uint8 bUnlimited : 1;

	// How deep can the combo tree go?
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 1, EditCondition = "!bUnlimited"))
		uint8 ComboTreeDepth = 1;

	// How long (in seconds) should we wait until we reset the combo? This value must be higher than the attack delays to create combos
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float ComboResetTime = 1.0f;

	// Log the current tree index
	UPROPERTY(EditInstanceOnly, Category = "Combo Tree")
		uint8 bLogComboTreeIndex : 1;

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
	FPlayerAttack_Data* AdvanceCombo_Internal(enum EAttackType_Player InAttackType);
	int8 AdvanceAttack(int8& AttackIndex, const TArray<FPlayerAttack_Data>& AttackList, const enum EAttackType_Player& InAttackType);

	void DelayAttack(const float& Delay);
	void LogAttackChain(); // Tracks the attacks we've chained

	void StoreAllMontageBlendTimes();

	EAttackType_Player CurrentAttack;

	//class UAnimMontage* CurrentAttackAnim;
	FPlayerAttack_Data* CurrentAttackData;

	// The attack chain history
	TArray<TEnumAsByte<EAttackType_Player>> Combo;
	TArray<TEnumAsByte<EAttackType_Player>> PreviousCombo;

	TArray<TArray<float>> OriginalBlendOutTimes;
	TArray<TArray<float>> OriginalBlendOutTriggerTimes;

	int8 LightAttackIndex = 0;
	int8 HeavyAttackIndex = 0;
	int8 SpecialAttackIndex = 0;

	int8 TreeIndex = 0;

	float OriginalComboMultiplier = 1.0f;

	FTimerHandle AttackDelayTimerHandle;
	FTimerHandle ComboResetTimerHandle;

	// The actor that owns this component
	AActor* Owner;
};
