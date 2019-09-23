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

void UMordathStageData::Init()
{
	Combat.GetAttackSettings().OriginalLightAttackDamage = Combat.GetAttackSettings().LightAttackDamage;
	Combat.GetAttackSettings().OriginalHeavyAttackDamage = Combat.GetAttackSettings().HeavyAttackDamage;
}

float UMordathStageData::GetShortAttackDamage()
{
	return Combat.GetAttackSettings().LightAttackDamage;
}

float UMordathStageData::GetLongAttackDamage()
{
	return Combat.GetAttackSettings().HeavyAttackDamage;
}

float UMordathStageData::GetSpecialAttackDamage()
{
	return Combat.GetAttackSettings().SpecialAttackDamage;
}

float UMordathStageData::GetAttackRadius()
{
	return Combat.GetAttackSettings().AttackRadius;
}

float UMordathStageData::GetAttackRadiusOnLowHealth()
{
	return Combat.GetAttackSettings().AttackRadiusOnLowHealth;
}

void UMordathStageData::SetAttackRadius(const float& NewRadius)
{
	Combat.GetAttackSettings().AttackRadius = NewRadius;
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
