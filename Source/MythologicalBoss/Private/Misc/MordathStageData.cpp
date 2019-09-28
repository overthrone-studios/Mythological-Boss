// Copyright Overthrone Studios 2019

#include "MordathStageData.h"

float FThinkStateData::CalculateThinkTime()
{
	const float Min = ThinkTime - RandomDeviation;
	const float Max = ThinkTime + RandomDeviation;

	return FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}

float FRetreatStateData::CalculateRetreatTime()
{
	const float Min = RetreatTime - RandomDeviation;
	const float Max = RetreatTime + RandomDeviation;

	return FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}

float UMordathStageData::GetAttackRadius()
{
	return Combat.AttackSettings.AttackRadius;
}

float UMordathStageData::GetRecentDamage()
{
	return Combat.RecentDamage;
}

bool UMordathStageData::IsHitStopEnabled()
{
	return Combat.bEnableHitStop;
}

float UMordathStageData::GetHitStopTime()
{
	return Combat.HitStopTime;
}
