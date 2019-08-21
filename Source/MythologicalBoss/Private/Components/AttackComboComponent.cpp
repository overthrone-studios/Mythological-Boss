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

uint8 UAttackComboComponent::AdvanceCombo(const EAttackType InAttackType)
{
	uint8 ComboIndex = 0;

	switch (InAttackType)
	{
	case Light:
		if (LightAttacks.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Light Attacks list", true);
			return TreeIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Light);
	break;

	case Heavy:
		if (HeavyAttacks.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Heavy Attacks list", true);
			return TreeIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Heavy);

	break;

	case Special:
		if (SpecialAttacks.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Special Attacks list", true);
			return TreeIndex;
		}

		ComboIndex = AdvanceCombo_Internal(Special);
	break;
	}

	return ComboIndex;
}

uint8 UAttackComboComponent::AdvanceCombo_Internal(const enum EAttackType InAttackType)
{
	if (TreeIndex >= ComboTreeDepth)
	{
		if (bDebug)
			ULog::Info("Reached the max tree depth, resetting...", true);

		ResetCombo();

		return TreeIndex;
	}

	// Start the combo reset timer
	Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 

	TreeIndex++;

	uint8 IndexToReturn = TreeIndex;

	// Choose the attack
	switch (InAttackType)
	{
	case Light:
		LightAttackIndex++;
		IndexToReturn = LightAttackIndex;

		Combo.Add(Light);
	break;

	case Heavy:
		HeavyAttackIndex++;
		IndexToReturn = HeavyAttackIndex;

		Combo.Add(Heavy);
	break;

	case Special:
		SpecialAttackIndex++;
		IndexToReturn = SpecialAttackIndex;

		Combo.Add(Special);
	break;
	}

	if (bDebug)
		ULog::Number(TreeIndex, "Combo Index: ", true);

	return IndexToReturn;
}

void UAttackComboComponent::ResetCombo()
{
	TreeIndex = 0;
	LightAttackIndex = 0;
	HeavyAttackIndex = 0;
	SpecialAttackIndex = 0;

	Combo.Empty();
	Owner->GetWorldTimerManager().ClearTimer(ComboResetTimerHandle);

	if (bDebug)
		ULog::Success("Combo reset!", true);
}
