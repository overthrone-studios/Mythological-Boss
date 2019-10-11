// Copyright Overthrone Studios 2019

#include "AttackComboComponent.h"

#include "GameFramework/Actor.h"

#include "Animation/AnimMontage.h"

#include "TimerManager.h"
#include "Log.h"

UAttackComboComponent::UAttackComboComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UAnimMontage* UAttackComboComponent::GetCurrentAttackAnim() const
{
	return CurrentAttackAnim;
}

FString UAttackComboComponent::GetCurrentAttackAsString() const
{
	switch (CurrentAttack)
	{
	case ATP_Light:
		return FString("Light");

	case ATP_Heavy:
		return FString("Heavy");

	case ATP_Special:
		return FString("Special");

	default:
		return FString("None");
	}
}

bool UAttackComboComponent::CanAttack() const
{
	return !Owner->GetWorldTimerManager().IsTimerActive(AttackDelayTimerHandle);
}

bool UAttackComboComponent::IsWaitingForComboReset() const
{
	return Owner->GetWorldTimerManager().IsTimerActive(ComboResetTimerHandle);
}

bool UAttackComboComponent::IsAtTreeEnd() const
{
	return TreeIndex >= ComboTreeDepth && !bUnlimited;
}

void UAttackComboComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();

	OriginalComboMultiplier = ComboMultiplier;
	CurrentAttack = ATP_None;

	StoreAllMontageBlendTimes();
}

class UAnimMontage* UAttackComboComponent::AdvanceCombo(const EAttackType_Player InAttackType)
{
	CurrentAttack = InAttackType;

	switch (InAttackType)
	{
	case ATP_Light:
		if (LightAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Light Attacks list", true);
			return nullptr;
		}

		CurrentAttackAnim = AdvanceCombo_Internal(ATP_Light);
	break;

	case ATP_Heavy:
		if (HeavyAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Heavy Attacks list", true);
			return nullptr;
		}

		CurrentAttackAnim = AdvanceCombo_Internal(ATP_Heavy);
	break;

	case ATP_Special:
		if (SpecialAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Special Attacks list", true);
			return nullptr;
		}

		CurrentAttackAnim = AdvanceCombo_Internal(ATP_Special);
	break;

	case ATP_None:
		CurrentAttackAnim = nullptr;
	break;
	}

	return CurrentAttackAnim;
}

class UAnimMontage* UAttackComboComponent::AdvanceCombo_Internal(const enum EAttackType_Player InAttackType)
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

	UAnimMontage* MontageToReturn = nullptr;

	// Choose the attack
	switch (InAttackType)
	{
	case ATP_Light:
		AdvanceAttack(LightAttackIndex, LightAttacks.List, ATP_Light);
		MontageToReturn = GetCurrentLightAttackAnim();

		LightAttackIndex++;
	break;

	case ATP_Heavy:
		AdvanceAttack(HeavyAttackIndex, HeavyAttacks.List, ATP_Heavy);
		MontageToReturn = GetCurrentHeavyAttackAnim();

		HeavyAttackIndex++;
	break;

	case ATP_Special:
		AdvanceAttack(SpecialAttackIndex, SpecialAttacks.List, ATP_Special);
		MontageToReturn = GetCurrentSpecialAttackAnim();

		SpecialAttackIndex++;
	break;

	case ATP_None:
		MontageToReturn = nullptr;
	break;	
	}

	TreeIndex++;

	if (bLogComboTreeIndex)
		ULog::Number(TreeIndex, "Tree Index: ", true);

	return MontageToReturn;
}

int8 UAttackComboComponent::AdvanceAttack(int8& AttackIndex, const TArray<class UAnimMontage*>& AttackList, const EAttackType_Player& InAttackType)
{
	if (AttackIndex >= AttackList.Num())
	{
		AttackIndex = 0;
	}

	Combo.Add(InAttackType);

	switch (InAttackType)
	{
	case ATP_Light:
		DelayAttack(LightAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Light Attack Index: ", true);
	break;

	case ATP_Heavy:
		DelayAttack(HeavyAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Heavy Attack Index: ", true);
	break;

	case ATP_Special:
		DelayAttack(SpecialAttacks.AttackDelay); 

		if (bLogAttackIndex)
			ULog::Number(AttackIndex, "Special Attack Index: ", true);
	break;

	case ATP_None:
	break;
	}

	return AttackIndex;
}

void UAttackComboComponent::DelayAttack(const float& Delay)
{
	if (CanAttack())
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

	CurrentAttack = ATP_None;

	// Save our attack chain
	PreviousCombo = Combo;

	// Clear for the next attack chain
	Combo.Empty();

	// Clear timers
	Owner->GetWorldTimerManager().ClearTimer(ComboResetTimerHandle);

	OnComboReset.Broadcast();

	if (bLogTreeStatus)
		ULog::Success("Combo reset!", true);
}

void UAttackComboComponent::ClearCurrentAttack()
{
	CurrentAttack = ATP_None;
}

void UAttackComboComponent::LogAttackChain()
{
	for (const auto Attack : Combo)
	{
		switch (Attack)
		{
			case ATP_Light:
				ULog::Success("Light", true);
			break;

			case ATP_Heavy:
				ULog::Success("Heavy", true);
			break;

			case ATP_Special:
				ULog::Success("Special", true);
			break;

			case ATP_None:
			break;

			default: 
			break;
		}
	}
}

void UAttackComboComponent::StoreAllMontageBlendTimes()
{
	TArray<TArray<UAnimMontage*>> AttackMontages;
	AttackMontages.Add(LightAttacks.List);
	AttackMontages.Add(HeavyAttacks.List);
	AttackMontages.Add(SpecialAttacks.List);

	OriginalBlendOutTimes.Init({}, AttackMontages.Num());
	OriginalBlendOutTriggerTimes.Init({}, AttackMontages.Num());

	for (int32 i = 0; i < AttackMontages.Num(); ++i)
	{
		OriginalBlendOutTimes[i].Init({}, AttackMontages[i].Num());
		OriginalBlendOutTriggerTimes[i].Init({}, AttackMontages[i].Num());
	}

	for (int32 i = 0; i < AttackMontages.Num(); ++i)
	{
		for (int32 j = 0; j < AttackMontages[i].Num(); ++j)
		{
			OriginalBlendOutTimes[i][j] = AttackMontages[i][j]->BlendOut.GetBlendTime();
			OriginalBlendOutTriggerTimes[i][j] = AttackMontages[i][j]->BlendOutTriggerTime;
		}
	}
}

void UAttackComboComponent::ResetAllMontageBlendTimes()
{
	TArray<TArray<UAnimMontage*>> AttackMontages;
	AttackMontages.Add(LightAttacks.List);
	AttackMontages.Add(HeavyAttacks.List);
	AttackMontages.Add(SpecialAttacks.List);

	for (int32 i = 0; i < AttackMontages.Num(); ++i)
	{
		for (int32 j = 0; j < AttackMontages[i].Num(); ++j)
		{
			AttackMontages[i][j]->BlendOut.SetBlendTime(OriginalBlendOutTimes[i][j]);
			AttackMontages[i][j]->BlendOutTriggerTime = OriginalBlendOutTriggerTimes[i][j];
		}
	}
}
