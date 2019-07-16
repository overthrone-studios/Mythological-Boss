// Copyright Overthrone Studios 2019

#include "FSM.h"

UFSM::UFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM::BeginPlay()
{
	Super::BeginPlay();

}

void UFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

