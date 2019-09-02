// Copyright Overthrone Studios 2019

#include "DashComponent.h"

#include "GameFramework/Actor.h"

#include "TimerManager.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDashComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

bool UDashComponent::IsCooldownActive()
{
	return Owner->GetWorldTimerManager().IsTimerActive(DashCooldownTimer);
}

float UDashComponent::GetCooldownTimeRemaining()
{
	return Owner->GetWorldTimerManager().GetTimerRemaining(DashCooldownTimer);
}

void UDashComponent::StartCooldown()
{
	Owner->GetWorldTimerManager().SetTimer(DashCooldownTimer, DashCooldown, false);
}
