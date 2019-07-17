// Copyright Overthrone Studios 2019

#include "FSM.h"
#include "Log.h"
#include "Player/Ylva.h"

UFSM::UFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM::BeginPlay()
{
	Super::BeginPlay();

}

void UFSM::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ActiveState->OnUpdateState.Broadcast();
	ActiveState->Uptime += DeltaTime;
}

void UFSM::InitState(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			PreviousState = &State;

			ActiveState = &State;
			ActiveState->OnEnterState.Broadcast();

			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("State does not exist"), true);
}

void UFSM::InitState(const FName& StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			ActiveState = &State;
			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("State does not exist"), true);
}

void UFSM::AddState(const int32 ID, const FName& StateName)
{
	const FState NewState(ID, StateName);

	States.Add(NewState);
}

FState* UFSM::GetState(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
			return &State;
	}

	ULog::LogDebugMessage(WARNING, FString("State does not exist"), true);
	return nullptr;
}

FState* UFSM::GetState(const FName& StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
			return &State;
	}

	ULog::LogDebugMessage(WARNING, FString("State does not exist"), true);
	return nullptr;
}

FState* UFSM::GetActiveState() const
{
	return ActiveState;
}

int32 UFSM::GetActiveStateID() const
{
	return ActiveState->ID;
}

FName UFSM::GetActiveStateName() const
{
	return ActiveState->Name;
}

FState* UFSM::GetPreviousState() const
{
	return PreviousState;
}

int32 UFSM::GetPreviousStateID() const
{
	return PreviousState->ID;
}

FName UFSM::GetPreviousStateName() const
{
	return PreviousState->Name;
}

float UFSM::GetPreviousStateUptime() const
{
	return PreviousState->Uptime;
}

float UFSM::GetActiveStateUptime() const
{
	return ActiveState->Uptime;
}

void UFSM::SetActiveState(const int32 StateID)
{
	// Exit if we are changing to the same state
	if (ActiveState->ID == StateID)
		return;

	// Does the state exist?
	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			PreviousState = ActiveState;

			ActiveState->OnExitState.Broadcast();
			ActiveState->Uptime = 0;
			ActiveState = &State;
			ActiveState->OnEnterState.Broadcast();

			return;
		}
	}
}

void UFSM::SetActiveState(const FName& StateName)
{
	// Exit if we are changing to the same state
	if (ActiveState->Name == StateName)
		return;

	for (FState& State : States)
	{
		// Does the state exist?
		if (State.Name == StateName)
		{
			PreviousState = ActiveState;

			ActiveState->OnExitState.Broadcast();
			ActiveState->Uptime = 0;
			ActiveState = &State;
			ActiveState->OnEnterState.Broadcast();

			return;
		}
	}
}
