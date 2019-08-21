// Copyright Overthrone Studios 2019

#include "AttackComboComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Log.h"

UAttackComboComponent::UAttackComboComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttackComboComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();
}

int32 UAttackComboComponent::AdvanceCombo(const EAttackType InAttackType)
{
	int8 ComboIndex = -1;

	switch (InAttackType)
	{
	case Light:
		if (LightAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Light Attacks list", true);
			return ComboIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Light);
	break;

	case Heavy:
		if (HeavyAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Heavy Attacks list", true);
			return ComboIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Heavy);

	break;

	case Special:
		if (SpecialAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Special Attacks list", true);
			return ComboIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Special);
	break;

	case None:
		ComboIndex = -1;
	break;
	}

	return ComboIndex;
}

int8 UAttackComboComponent::AdvanceCombo_Internal(const enum EAttackType InAttackType)
{
	if (TreeIndex >= ComboTreeDepth && Owner->GetWorldTimerManager().IsTimerActive(ComboResetTimerHandle))
	{
		if (bLogTreeStatus)
			ULog::Info("Reached the max tree depth, resetting...", true);

		return -1;
	}

	if (TreeIndex >= ComboTreeDepth && !Owner->GetWorldTimerManager().IsTimerActive(ComboResetTimerHandle))
	{
		Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 
		return -1;
	}

	// Start the combo reset timer
	Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 

	// Get out if we are still delaying
	if (Owner->GetWorldTimerManager().IsTimerActive(AttackDelayTimerHandle))
		return -1;

	TreeIndex++;

	int8 IndexToReturn = TreeIndex;

	// Choose the attack
	switch (InAttackType)
	{
	case Light:
		IndexToReturn = AdvanceAttack(LightAttackIndex, LightAttacks.List, Light);
	break;

	case Heavy:
		IndexToReturn = AdvanceAttack(HeavyAttackIndex, HeavyAttacks.List, Heavy);
	break;

	case Special:
		IndexToReturn = AdvanceAttack(SpecialAttackIndex, SpecialAttacks.List, Special);
	break;

	case None:
		IndexToReturn = -1;
	break;	
	}

	if (bLogComboIndex)
		ULog::Number(TreeIndex, "Combo Index: ", true);

	return IndexToReturn;
}

int8 UAttackComboComponent::AdvanceAttack(int8& AttackIndex, const TArray<class UAnimMontage*>& AttackList, const EAttackType& InAttackType)
{
	AttackIndex++;

	if (AttackIndex > AttackList.Num())
		AttackIndex = 1;

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

	if (bLogAttackChain)
		LogAttackChain();

	// Save our attack chain
	PreviousCombo = Combo;

	// Clear for the next attack chain
	Combo.Empty();

	// Clear timers
	Owner->GetWorldTimerManager().ClearTimer(ComboResetTimerHandle);
	Owner->GetWorldTimerManager().ClearTimer(AttackDelayTimerHandle);

	if (bLogTreeStatus)
		ULog::Success("Combo reset!", true);
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
