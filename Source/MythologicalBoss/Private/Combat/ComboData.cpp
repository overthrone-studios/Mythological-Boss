// Copyright Overthrone Studios 2019

#include "Combat/ComboData.h"
#include "Log.h"

void UComboData::Init()
{
	AttackIndex = 0;
	CurrentAttack = Attacks[0];

	ULog::DebugMessage(INFO, "Current Attack: " + FString::FromInt(AttackIndex), true);
}

void UComboData::NextAttack()
{
	AttackIndex++;

	if (AttackIndex >= Attacks.Num())
		AttackIndex = Attacks.Num();

	if (AttackIndex < Attacks.Num())
	{
		CurrentAttack = Attacks[AttackIndex];
		ULog::DebugMessage(INFO, "Current Attack: " + FString::FromInt(AttackIndex), true);
	}
}

bool UComboData::IsAtLastAttack()
{
	return AttackIndex == Attacks.Num();
}
