// Copyright Overthrone Studios 2019

#include "Components/ChargeAttackComponent.h"

UChargeAttackComponent::UChargeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxCharge = 100.0f;

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
	Charge = MaxCharge;
	PreviousCharge = MaxCharge;
	NewCharge = MaxCharge;
}
