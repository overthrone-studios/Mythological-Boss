// Copyright Overthrone Studios 2019

#include "Components/ChargeAttackComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UChargeAttackComponent::UChargeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxCharge = 100.0f;

	InitChargeAttack();
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
}

void UChargeAttackComponent::DecreaseCharge(const float Amount)
{
	PreviousCharge = Charge;

	Charge = FMath::Clamp(Charge - Amount, 0.0f, MaxCharge);
	SmoothedCharge = Charge;
}

void UChargeAttackComponent::ResetCharge()
{
	InitChargeAttack();
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
