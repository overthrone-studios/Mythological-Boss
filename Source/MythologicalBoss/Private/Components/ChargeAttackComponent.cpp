// Copyright Overthrone Studios 2019

#include "Components/ChargeAttackComponent.h"

#include "Misc/YlvaDifficultyData.h"

#include "GameFramework/Actor.h"

#include "TimerManager.h"

UChargeAttackComponent::UChargeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxCharge = 100.0f;

	InitChargeAttack();
}

void UChargeAttackComponent::InitChargeAttackEconomyValues(const UYlvaDifficultyData& DifficultyData)
{
	ChargeGainPerLightHit = DifficultyData.ChargeGainPerLightHit;
	ChargeGainPerHeavyHit = DifficultyData.ChargeGainPerHeavyHit;
	ChargeLossPerHit = DifficultyData.ChargeLossPerHit;
	HealthGainOnChargeAttack = DifficultyData.HealthGainOnChargeAttack;
	MaxChargeHoldTime = DifficultyData.MaxChargeHoldTime;
	ChargeHoldFrames = DifficultyData.ChargeHoldFrames;
	bResetChargeAfterMaxHits = DifficultyData.bResetChargeAfterMaxHits;
	MaxHitsForChargeReset = DifficultyData.MaxHitsForChargeReset;
	bLoseChargeOvertime = DifficultyData.bLoseChargeOvertime;
	DelayBeforeChargeLoss = DifficultyData.DelayBeforeChargeLoss;
	ChargeLossRate = DifficultyData.ChargeLossRate;
}

void UChargeAttackComponent::DelayChargeLoss()
{
	Owner->GetWorldTimerManager().SetTimer(ChargeLossTimerHandle, DelayBeforeChargeLoss, false);
}

bool UChargeAttackComponent::IsLosingCharge()
{
	return !Owner->GetWorldTimerManager().IsTimerActive(ChargeLossTimerHandle);
}

void UChargeAttackComponent::SetCharge(const float NewChargeAmount)
{
	PreviousCharge = Charge;

	Charge = FMath::Clamp(NewChargeAmount, 0.0f, MaxCharge);
	SmoothedCharge = Charge;

	if (Charge >= GetMaxCharge())
		OnChargeFilled.Broadcast();
	else if (Charge <= 0.0f)
		OnChargeEmpty.Broadcast();
}

void UChargeAttackComponent::SetSmoothedCharge(const float Value)
{
	SmoothedCharge = Value;
}

void UChargeAttackComponent::IncreaseCharge(const float Amount)
{
	PreviousCharge = Charge;

	Charge = FMath::Clamp(Charge + Amount, 0.0f, MaxCharge);
	SmoothedCharge = Charge;

	if (Charge >= GetMaxCharge())
		OnChargeFilled.Broadcast();
}

void UChargeAttackComponent::DecreaseCharge(const float Amount)
{
	PreviousCharge = Charge;

	Charge = FMath::Clamp(Charge - Amount, 0.0f, MaxCharge);
	SmoothedCharge = Charge;

	if (Charge <= 0.0f)
		OnChargeEmpty.Broadcast();
}

void UChargeAttackComponent::ResetCharge()
{
	InitChargeAttack();

	OnChargeEmpty.Broadcast();
}

void UChargeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	InitChargeAttack();
}

void UChargeAttackComponent::InitChargeAttack()
{
	Charge = 0.0f;
	PreviousCharge = 0.0f;
	SmoothedCharge = 0.0f;
}
