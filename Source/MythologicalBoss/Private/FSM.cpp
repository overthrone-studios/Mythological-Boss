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

#if !UE_BUILD_SHIPPING
	if (bDebug)
	{
		ULog::DebugMessage(SUCCESS, FString("FSM Initialized"));
	}
#endif
}

void UFSM::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(INFO, GetActiveStateName().ToString(), true);
#endif

	if (bIsRunning)
	{
		Stack[0]->OnUpdateState.Broadcast();
		OnUpdateAnyState.Broadcast();
		Stack[0]->Uptime += DeltaTime;
		Stack[0]->Frames++;
	}
}

void UFSM::Start()
{
#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString(GetName() + " has started"));
#endif

	Stack[0]->OnEnterState.Broadcast();
	OnEnterAnyState.Broadcast();
	bIsRunning = true;
}

void UFSM::Stop()
{
#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString(GetName() + " has stopped"));
#endif

	Stack[0]->OnExitState.Broadcast();
	OnExitAnyState.Broadcast();
	bIsRunning = false;
}

void UFSM::InitState(const int32 StateID)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("InitState: FSM has already been initialized. This should be called once!"), true);
#endif
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

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
#endif
}

void UFSM::InitState(const FName& StateName)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("InitState: FSM has already been initialized. This should be called once!"), true);
#endif

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

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
#endif
}

void UFSM::AddState(const int32 ID, const FName& StateName)
{
	States.Add({ID, StateName});

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(INFO, FString("State " + StateName.ToString() + " added"), true);
#endif
}

void UFSM::PopState()
{
	if (GetStateCountInStack() == 1)
		return;

	PopState(GetActiveStateID());
}

void UFSM::RemoveAllStatesFromStack()
{
	for (int32 i = 0; i < Stack.Num(); i++)
		PopState();

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for one"), true);
#endif
}

void UFSM::RemoveAllStatesFromStackExceptActive()
{
	for (int32 i = 0; i < Stack.Num(); i++)
	{
		if (Stack[i]->ID != GetActiveStateID())
			PopState();
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for the active state"), true);
#endif
}

void UFSM::PushState(const int32 StateID)
{
	if (DoesStateExistInStack(StateID))
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(INFO, FString("PushState: State ") + FString::FromInt(StateID) + FString(" already exists in the stack."), true);
#endif
		return;
	}

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			Stack[0]->OnExitState.Broadcast();
			OnExitAnyState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack[0]->Frames = 0;
			Stack.Insert(&State, 0);
			Stack[0]->OnEnterState.Broadcast();
			OnEnterAnyState.Broadcast();

			return;
		}
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PushState: State " + FString::FromInt(StateID) + " does not exist"), true);
#endif
}

void UFSM::PushState(const FName& StateName)
{
	if (DoesStateExistInStack(StateName))
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(INFO, FString("PushState: State ") + StateName.ToString() + FString(" already exists in the stack."), true);
#endif
		
		return;
	}

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			Stack[0]->OnExitState.Broadcast();
			OnExitAnyState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack[0]->Frames = 0;
			Stack.Insert(&State, 0);
			Stack[0]->OnEnterState.Broadcast();
			OnEnterAnyState.Broadcast();

			return;
		}
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PushState: State " + StateName.ToString() + " does not exist"), true);
#endif
}

void UFSM::PopState(const int32 StateID)
{
	if (Stack.Num() == 1)
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop state ") + FString::FromInt(StateID) + FString(" from the stack. At least one state should be in the stack!"), true);
#endif
		return;
	}

	for (FState* State : Stack)
	{
		if (State->ID == StateID)
		{
			Stack[0]->OnExitState.Broadcast();
			OnExitAnyState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack[0]->Frames = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();
			OnEnterAnyState.Broadcast();

			return;
		}
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PopState: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);
#endif
}

void UFSM::PopState(const FName& StateName)
{
	if (Stack.Num() == 1)
	{
#if !UE_BUILD_SHIPPING
		if (bDebug)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop ") + StateName.ToString() + FString(" state from the stack. At least one state should be in the stack!"), true);
#endif
		return;
	}

	for (FState* State : Stack)
	{
		if (State->Name == StateName)
		{
			Stack[0]->OnExitState.Broadcast();
			OnExitAnyState.Broadcast();
			Stack[0]->Uptime = 0;
			Stack[0]->Frames = 0;
			Stack.Remove(State);
			Stack[0]->OnEnterState.Broadcast();
			OnEnterAnyState.Broadcast();

			return;	
		}
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("PopState: State ") + StateName.ToString() + FString(" does not exist"), true);
#endif
}

FState* UFSM::GetState(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
			return &State;
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetState: State ") + FString::FromInt(StateID) + FString(" does not exist"), true);
#endif
	return nullptr;
}

FState* UFSM::GetState(const FName& StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
			return &State;
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetState: State ") + StateName.ToString() + FString(" does not exist"), true);
#endif

	return nullptr;
}

FState* UFSM::GetStateInStack(const int32 StateID)
{
	for (FState* State : Stack)
	{
		if (State->ID == StateID)
			return State;	
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetStateInStack: State ID ") + FString::FromInt(StateID) + FString(" does not exist"), true);
#endif
	return nullptr;
}

FState* UFSM::GetStateInStack(const FName& StateName)
{
	for (FState* State : Stack)
	{
		if (State->Name == StateName)
			return State;	
	}

#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(WARNING, FString("GetStateInStack: State ") + StateName.ToString() + FString(" does not exist"), true);
#endif
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

int32 UFSM::GetActiveStateFrames() const
{
	return Stack[0]->Frames;
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
