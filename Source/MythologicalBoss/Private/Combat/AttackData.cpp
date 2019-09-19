// Copyright Overthrone Studios 2019

#include "AttackData.h"

FString UAttackData::GetCounterTypeAsString() const
{
	switch (CounterType)
	{
	case Parryable:
		return FString("Parryable");

	case Blockable:
		return FString("Blockable");

	case NoCounter:
		return FString("No Counter");

	default:
		return FString("None");
	}
}

FString UAttackData::GetCurrentAttackAsString() const
{
	// Choose the current attack from the attack data
	switch (Attack)
	{
	case ShortAttack_1:
		return FString("Short Attack 1");

	case ShortAttack_2:
		return FString("Short Attack 2");

	case ShortAttack_3:
		return FString("Short Attack 3");

	case LongAttack_1:
		return FString("Long Attack 1");

	case LongAttack_2:
		return FString("Long Attack 2");

	case LongAttack_3:
		return FString("Long Attack 3");

	default:
		return FString("None");
	}
}
