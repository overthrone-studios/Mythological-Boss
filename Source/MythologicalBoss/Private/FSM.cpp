// Copyright Overthrone Studios 2019

#include "FSM.h"
#include "Log.h"
#include "Player/Ylva.h"
#include "Kismet/KismetSystemLibrary.h"

UFSM::UFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSM::BeginPlay()
{
	Super::BeginPlay();

	if (!bHasFSMInitialized)
	{
		ULog::DebugMessage(ERROR, FString(GetOwner()->GetName() + "'s FSM has not been initialized! Initialize its FSM component in the owner's BeginPlay function."));
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Background);
		return;
	}

	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString("FSM Initialized"));
}

void UFSM::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Stack[0]->OnUpdateState.Broadcast();
	Stack[0]->Uptime += DeltaTime;
}

void UFSM::Start()
{
	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString("FSM has started"));

	Stack[0]->OnEnterState.Broadcast();
}

void UFSM::InitState(const int32 StateID)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("InitState: FSM has already been initialized. This should be called once!"), true);

		return;
	}

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			Stack.Add(&State);

			bHasFSMInitialized = true;
			return;
		}
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
}

void UFSM::InitState(const FName& StateName)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("InitState: FSM has already been initialized. This should be called once!"), true);

		return;
	}

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			Stack.Add(&State);

			bHasFSMInitialized = true;
			return;
		}
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
}

void UFSM::AddState(const int32 ID, const FName& StateName)
{
	States.Add({ID, StateName});

	if (bDebug)
		ULog::DebugMessage(INFO, FString("State " + StateName.ToString() + " added"), true);
}

void UFSM::PopState()
{
	if (GetStateCountInStack() == 1)
		return;

	PopState(GetActiveStateID());
}

void UFSM::RemoveAllStatesFromStack()
{
	for (int32 i = 1; i < Stack.Num(); i++)
		Stack.RemoveAt(i);

	if (bDebug)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for one"), true);
}

void UFSM::PushState(const int32 StateID)
{
	if (DoesStateExistInStack(StateID))
	{
		if (bDebug)
			ULog::DebugMessage(INFO, FString("PushState: State ") + FString::FromInt(StateID) + FString(" already exists in the stack."), true);
		
		return;
	}

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

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PushState: State " + FString::FromInt(StateID) + " does not exist"), true);
}

void UFSM::PushState(const FName& StateName)
{
	if (DoesStateExistInStack(StateName))
	{
		if (bDebug)
			ULog::DebugMessage(INFO, FString("PushState: State ") + StateName.ToString() + FString(" already exists in the stack."), true);
		
		return;
	}

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

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PushState: State " + StateName.ToString() + " does not exist"), true);
}

void UFSM::PopState(const int32 StateID)
{
	if (Stack.Num() == 1)
	{
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop state ") + FString::FromInt(StateID) + FString(" from the stack. At least one state should be in the stack!"), true);

		return;
	}

	for (FState* State : Stack)
	{
		if (State->ID == StateID)
		{
			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();

			return;
		}
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PopState: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);
}

void UFSM::PopState(const FName& StateName)
{
	if (Stack.Num() == 1)
	{
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop ") + StateName.ToString() + FString(" state from the stack. At least one state should be in the stack!"), true);

		return;
	}

	for (FState* State : Stack)
	{
		if (State->Name == StateName)
		{
			Stack[0]->OnExitState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();

			return;	
		}
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PopState: State ") + StateName.ToString() + FString(" does not exist"), true);
}

FState* UFSM::GetState(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
			return &State;
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetState: State ") + FString::FromInt(StateID) + FString(" does not exist"), true);

	return nullptr;
}

FState* UFSM::GetState(const FName& StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
			return &State;
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetState: State ") + StateName.ToString() + FString(" does not exist"), true);

	return nullptr;
}

FState* UFSM::GetStateInStack(const int32 StateID)
{
	for (FState* State : Stack)
	{
		if (State->ID == StateID)
			return State;	
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetStateInStack: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);

	return nullptr;
}

FState* UFSM::GetStateInStack(const FName& StateName)
{
	for (FState* State : Stack)
	{
		if (State->Name == StateName)
			return State;	
	}

	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetStateInStack: State ") + StateName.ToString() + FString(" does not exist"), true);

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

bool UFSM::IsStackEmpty() const
{
	return Stack.Num() == 0;
}
