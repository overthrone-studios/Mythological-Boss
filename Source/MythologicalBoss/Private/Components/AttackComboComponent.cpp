// Copyright Overthrone Studios 2019

#include "AttackComboComponent.h"


UAttackComboComponent::UAttackComboComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UAttackComboComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UAttackComboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

