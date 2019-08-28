// Copyright Overthrone Studios 2019

#include "AttackComboComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Log.h"

UAttackComboComponent::UAttackComboComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OnAttackEnd.AddDynamic(this, &UAttackComboComponent::ClearCurrentAttack);
}

bool UAttackComboComponent::IsDelaying() const
{
	return Owner->GetWorldTimerManager().IsTimerActive(AttackDelayTimerHandle);
}

bool UAttackComboComponent::IsWaitingForComboReset() const
{
	return Owner->GetWorldTimerManager().IsTimerActive(ComboResetTimerHandle);
}

bool UAttackComboComponent::IsAtTreeEnd() const
{
	return TreeIndex >= ComboTreeDepth;
}

void UAttackComboComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();

	OriginalComboMultiplier = ComboMultiplier;
}

class UAnimMontage* UAttackComboComponent::AdvanceCombo(const EAttackType InAttackType)
{
	UAnimMontage* AttackMontage = nullptr;

	CurrentAttack = InAttackType;

	switch (InAttackType)
	{
	case Light:
		if (LightAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Light Attacks list", true);
			return AttackMontage;
		}

		AttackMontage = AdvanceCombo_Internal(Light);
	break;

	case Heavy:
		if (HeavyAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Heavy Attacks list", true);
			return AttackMontage;
		}

		AttackMontage = AdvanceCombo_Internal(Heavy);

	break;

	case Special:
		if (SpecialAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Special Attacks list", true);
			return AttackMontage;
		}

		AttackMontage = AdvanceCombo_Internal(Special);
	break;

	case None:
		AttackMontage = nullptr;
	break;
	}

	return AttackMontage;
}

class UAnimMontage* UAttackComboComponent::AdvanceCombo_Internal(const enum EAttackType InAttackType)
{
	if (IsAtTreeEnd() && IsWaitingForComboReset())
	{
		if (bLogTreeStatus)
			ULog::Info("Reached the max tree depth, resetting...", true);

		return nullptr;
	}

	if (IsAtTreeEnd() && !IsWaitingForComboReset())
	{
		Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 
		return nullptr;
	}

	// Increment the combo multiplier after the first attack
	if (TreeIndex > 0 && TreeIndex <= ComboMultiplierCount)
		ComboMultiplier += MultiplierIncrementAmount;
	else
		ComboMultiplier = OriginalComboMultiplier;

	// Start the combo reset timer
	Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 

	// Get out if we are still delaying
	if (IsDelaying())
		return nullptr;

	UAnimMontage* MontageToReturn = nullptr;

	// Choose the attack
	switch (InAttackType)
	{
	case Light:
		AdvanceAttack(LightAttackIndex, LightAttacks.List, Light);
		MontageToReturn = GetCurrentLightAttackAnim();

		LightAttackIndex++;
	break;

	case Heavy:
		AdvanceAttack(HeavyAttackIndex, HeavyAttacks.List, Heavy);
		MontageToReturn = GetCurrentHeavyAttackAnim();

		HeavyAttackIndex++;
	break;

	case Special:
		AdvanceAttack(SpecialAttackIndex, SpecialAttacks.List, Special);
		MontageToReturn = GetCurrentSpecialAttackAnim();

		SpecialAttackIndex++;
	break;

	case None:
		MontageToReturn = nullptr;
	break;	
	}

	TreeIndex++;

	if (bLogComboTreeIndex)
		ULog::Number(TreeIndex, "Tree Index: ", true);

	return MontageToReturn;
}

int8 UAttackComboComponent::AdvanceAttack(int8& AttackIndex, const TArray<class UAnimMontage*>& AttackList, const EAttackType& InAttackType)
{
	if (AttackIndex >= AttackList.Num())
	{
		AttackIndex = 0;
		return AttackIndex;
	}

	Combo.Add(InAttackType);

	switch (InAttackType)
	{
	case Light:
		DelayAttack(LightAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Light Attack Index: ", true);
	break;

	case Heavy:
		DelayAttack(HeavyAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Heavy Attack Index: ", true);
	break;

	case Special:
		DelayAttack(SpecialAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Special Attack Index: ", true);
	break;

	case None:
	break;
	}

	return AttackIndex;
}

void UAttackComboComponent::DelayAttack(const float& Delay)
{
	if (!Owner->GetWorldTimerManager().IsTimerActive(AttackDelayTimerHandle))
		Owner->GetWorldTimerManager().SetTimer(AttackDelayTimerHandle, Delay, false);
}

void UAttackComboComponent::ResetCombo()
{
	// Clear indices
	TreeIndex = 0;
	LightAttackIndex = 0;
	HeavyAttackIndex = 0;
	SpecialAttackIndex = 0;

	ComboMultiplier = OriginalComboMultiplier;

	if (bLogAttackChain)
		LogAttackChain();

	CurrentAttack = None;

	// Save our attack chain
	PreviousCombo = Combo;

	// Clear for the next attack chain
	Combo.Empty();

	// Clear timers
	Owner->GetWorldTimerManager().ClearTimer(ComboResetTimerHandle);
	Owner->GetWorldTimerManager().ClearTimer(AttackDelayTimerHandle);

	OnComboReset.Broadcast();

	if (bLogTreeStatus)
		ULog::Success("Combo reset!", true);
}

void UAttackComboComponent::ClearCurrentAttack()
{
	CurrentAttack = None;
}

void UAttackComboComponent::LogAttackChain()
{
	for (const auto Attack : Combo)
	{
		switch (Attack)
		{
			case Light:
				ULog::Success("Light", true);
			break;

			case Heavy:
				ULog::Success("Heavy", true);
			break;

			case Special:
				ULog::Success("Special", true);
			break;

			case None:
			break;

			default: 
			break;
		}
	}
}
