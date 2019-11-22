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

FPlayerAttack_Data* UAttackComboComponent::AdvanceCombo(const EAttackType_Player InAttackType)
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

		CurrentAttackData = AdvanceCombo_Internal(ATP_Light);
	break;

	case ATP_Heavy:
		if (HeavyAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Heavy Attacks list", true);
			return nullptr;
		}

		CurrentAttackData = AdvanceCombo_Internal(ATP_Heavy);
	break;

	case ATP_Special:
		if (SpecialAttacks.List.Num() == 0)
		{
			ULog::Error("Could not advance the combo tree. There must be at least 1 attack in the Special Attacks list", true);
			return nullptr;
		}

		CurrentAttackData = AdvanceCombo_Internal(ATP_Special);
	break;

	case ATP_None:
		CurrentAttackData = nullptr;
	break;
	}

	return CurrentAttackData;
}

FPlayerAttack_Data* UAttackComboComponent::AdvanceCombo_Internal(const enum EAttackType_Player InAttackType)
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
	{
		ComboMultiplier = OriginalComboMultiplier;
		TreeIndex = 0;
		OnComboMultiplierReached.Broadcast();
	}

	// Start the combo reset timer
	Owner->GetWorldTimerManager().SetTimer(ComboResetTimerHandle, this, &UAttackComboComponent::ResetCombo, ComboResetTime, false); 

	UAnimMontage* MontageToReturn = nullptr;

	// Choose the attack
	switch (InAttackType)
	{
	case ATP_Light:
		AdvanceAttack(LightAttackIndex, LightAttacks.List, ATP_Light);
		CurrentAttackData = &GetCurrentLightAttackData();
		//MontageToReturn = GetCurrentLightAttackAnim();

		LightAttackIndex++;
	break;

	case ATP_Heavy:
		AdvanceAttack(HeavyAttackIndex, HeavyAttacks.List, ATP_Heavy);
		CurrentAttackData = &GetCurrentHeavyAttackData();
		//MontageToReturn = GetCurrentHeavyAttackAnim();

		HeavyAttackIndex++;
	break;

	case ATP_Special:
		AdvanceAttack(SpecialAttackIndex, SpecialAttacks.List, ATP_Special);
		CurrentAttackData = &GetCurrentSpecialAttackData();
		//MontageToReturn = GetCurrentSpecialAttackAnim();

		SpecialAttackIndex++;
	break;

	case ATP_None:
		CurrentAttackData = nullptr;
		//MontageToReturn = nullptr;
	break;	
	}

	TreeIndex++;

	if (bLogComboTreeIndex)
		ULog::Number(TreeIndex, "Tree Index: ", true);

	return CurrentAttackData;
}

int8 UAttackComboComponent::AdvanceAttack(int8& AttackIndex, const TArray<FPlayerAttack_Data>& AttackList, const EAttackType_Player& InAttackType)
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

#if !UE_BUILD_SHIPPING
	if (bLogTreeStatus)
		ULog::Success("Combo reset!", true);
#endif
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
	TArray<TArray<FPlayerAttack_Data>> AttackDataArray;
	AttackDataArray.Add(LightAttacks.List);
	AttackDataArray.Add(HeavyAttacks.List);
	AttackDataArray.Add(SpecialAttacks.List);

	OriginalBlendOutTimes.Init({}, AttackDataArray.Num());
	OriginalBlendOutTriggerTimes.Init({}, AttackDataArray.Num());
	OriginalBlendOutBlendOptions.Init({}, AttackDataArray.Num());

	for (int32 i = 0; i < AttackDataArray.Num(); ++i)
	{
		OriginalBlendOutTimes[i].Init({}, AttackDataArray[i].Num());
		OriginalBlendOutTriggerTimes[i].Init({}, AttackDataArray[i].Num());
		OriginalBlendOutBlendOptions[i].Init({}, AttackDataArray[i].Num());
	}

	for (int32 i = 0; i < AttackDataArray.Num(); ++i)
	{
		for (int32 j = 0; j < AttackDataArray[i].Num(); ++j)
		{
			OriginalBlendOutTimes[i][j] = AttackDataArray[i][j].AttackMontage->BlendOut.GetBlendTime();
			OriginalBlendOutTriggerTimes[i][j] = AttackDataArray[i][j].AttackMontage->BlendOutTriggerTime;
			OriginalBlendOutBlendOptions[i][j] = AttackDataArray[i][j].AttackMontage->BlendOut.GetBlendOption();
		}
	}
}

void UAttackComboComponent::ApplyBlendOutSettings()
{
	if (CurrentAttackData->bBlendOutOnLowStamina)
	{
		CurrentAttackData->AttackMontage->BlendOut.SetBlendTime(CurrentAttackData->BlendOutTimeOnLowStamina);
		CurrentAttackData->AttackMontage->BlendOutTriggerTime = CurrentAttackData->BlendOutTriggerTimeOnLowStamina;
		CurrentAttackData->AttackMontage->BlendOut.SetBlendOption(CurrentAttackData->BlendOption);
	}
}

void UAttackComboComponent::ResetAllBlendOutSettings()
{
	TArray<TArray<FPlayerAttack_Data>> AttackDataArray;
	AttackDataArray.Add(LightAttacks.List);
	AttackDataArray.Add(HeavyAttacks.List);
	AttackDataArray.Add(SpecialAttacks.List);

	for (int32 i = 0; i < AttackDataArray.Num(); ++i)
	{
		for (int32 j = 0; j < AttackDataArray[i].Num(); ++j)
		{
			AttackDataArray[i][j].AttackMontage->BlendOut.SetBlendTime(OriginalBlendOutTimes[i][j]);
			AttackDataArray[i][j].AttackMontage->BlendOutTriggerTime = OriginalBlendOutTriggerTimes[i][j];
			AttackDataArray[i][j].AttackMontage->BlendOut.SetBlendOption(OriginalBlendOutBlendOptions[i][j]);
		}
	}
}
