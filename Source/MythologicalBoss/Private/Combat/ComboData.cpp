// Copyright Overthrone Studios 2019

#include "Combat/ComboData.h"


void UComboData::Init()
{
	AttackIndex = 0;
	CurrentAttack = Attacks[0];
}

void UComboData::NextAttack()
{
	AttackIndex++;

	if (AttackIndex >= Attacks.Num())
		AttackIndex = Attacks.Num();

	if (AttackIndex < Attacks.Num())
	{
		CurrentAttack = Attacks[AttackIndex];
	}
}

bool UComboData::IsAtLastAttack()
{
	return AttackIndex == Attacks.Num();
}

bool UComboData::IsDelayEnabled()
{
	return bDelayBetweenAttacks;
}

float UComboData::GetDelayTime()
{
	return Delay;
}

float UComboData::GetDeviation()
{
	return RandomDeviation;
}

FTimerHandle& UComboData::GetDelayTimer()
{
	return DelayTimerHandle;
}
