// Copyright Overthrone Studios 2019

#include "AttackData.h"

FString UAttackData::GetCounterTypeAsString()
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
