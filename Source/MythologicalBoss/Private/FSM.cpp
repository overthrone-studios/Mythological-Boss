// Copyright Overthrone Studios 2019

#include "FSM.h"
#include "Log.h"
#include "Player/Ylva.h"

UFSM::UFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFSM::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Stack[0]->OnUpdateState.Broadcast();
	Stack[0]->Uptime += DeltaTime;
}

void UFSM::InitState(const int32 StateID)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
		return;

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			Stack.Add(&State);
			Stack[0]->OnEnterState.Broadcast();

			bHasFSMInitialized = true;
			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("InitState: State does not exist"), true);
}

void UFSM::InitState(const FName& StateName)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
		return;

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			Stack.Add(&State);
			Stack[0]->OnEnterState.Broadcast();

			bHasFSMInitialized = true;
			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("InitState: State does not exist"), true);
}

void UFSM::AddState(const int32 ID, const FName& StateName)
{
	const FState NewState(ID, StateName);

	States.Add(NewState);
}

void UFSM::PopState()
{
	if (GetStateCountInStack() == 1)
		return;

	PopState(GetActiveStateID());
}

void UFSM::PushState(const int32 StateID)
{
	if (DoesStateExistInStack(StateID))
		return;

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Insert(&State, 0);
			Stack[0]->OnEnterState.Broadcast();

			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("PushState: State does not exist"), true);
}

void UFSM::PushState(const FName& StateName)
{
	if (DoesStateExistInStack(StateName))
		return;

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Insert(&State, 0);
			Stack[0]->OnEnterState.Broadcast();

			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("PushState: State does not exist"), true);
}

void UFSM::PopState(const int32 StateID)
{
	for (FState* State : Stack)
	{
		if (State->ID == StateID)
		{
			if (Stack.Num() == 1)
			{
				ULog::LogDebugMessage(WARNING, FString("PopState: Cannot pop state ") + FString::FromInt(StateID) + FString(" from the stack. At least one state should be in the stack!"), true);
				return;
			}

			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();

			return;
		}
	}

	ULog::LogDebugMessage(WARNING, FString("PopState: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);
}

void UFSM::PopState(const FName& StateName)
{
	for (FState* State : Stack)
	{
		if (State->Name == StateName)
		{
			if (Stack.Num() == 1)
			{
				ULog::LogDebugMessage(WARNING, FString("PopState: Cannot pop ") + StateName.ToString() + FString(" state from the stack. At least one state should be in the stack!"), true);
				return;
			}

			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();

			return;	
		}
	}

	ULog::LogDebugMessage(WARNING, FString("PopState: State ") + StateName.ToString() + FString(" does not exist"), true);
}

FState* UFSM::GetState(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
			return &State;
	}

	ULog::LogDebugMessage(WARNING, FString("GetState: State does not exist"), true);
	return nullptr;
}

FState* UFSM::GetState(const FName& StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
			return &State;
	}

	ULog::LogDebugMessage(WARNING, FString("GetState: State does not exist"), true);
	return nullptr;
}

FState* UFSM::GetStateInStack(const int32 StateID)
{
	for (FState* State : Stack)
	{
		if (State->ID == StateID)
			return State;	
	}

	ULog::LogDebugMessage(WARNING, FString("GetStateInStack: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);
	return nullptr;
}

FState* UFSM::GetStateInStack(const FName& StateName)
{
	for (FState* State : Stack)
	{
		if (State->Name == StateName)
			return State;	
	}

	ULog::LogDebugMessage(WARNING, FString("GetStateInStack: State ") + StateName.ToString() + FString(" does not exist"), true);
	return nullptr;
}

FState* UFSM::GetActiveState() const
{
	return Stack[0];
}

int32 UFSM::GetActiveStateID() const
{
	return Stack[0]->ID;
}

FName UFSM::GetActiveStateName() const
{
	return Stack[0]->Name;
}

float UFSM::GetActiveStateUptime() const
{
	return Stack[0]->Uptime;
}

bool UFSM::DoesStateExistInStack(const int32 StateID)
{
	for (FState* State : Stack)
	{
		return State->ID == StateID;
	}

	return false;
}

bool UFSM::DoesStateExistInStack(const FName& StateName)
{
	for (FState* State : Stack)
	{
		return State->Name == StateName;
	}

	return false;
}
