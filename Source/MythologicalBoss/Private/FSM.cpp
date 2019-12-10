// Copyright Ali El Saleh 2019

#include "FSM.h"

#include "Log.h"

UFSM::UFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	History.AddDefaulted();
}

void UFSM::BeginPlay()
{
	Super::BeginPlay();

	#if !UE_BUILD_SHIPPING
	if (bDebug)
	{
		ULog::DebugMessage(SUCCESS, FString("FSM Initialized"), true);
	}
	#endif
}

void UFSM::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, GetActiveStateName().ToString(), true);
	#endif

	if (bIsRunning)
	{
		Stack[0]->OnUpdateState.Broadcast(Stack[0]->Uptime, Stack[0]->Frames);
		OnUpdateAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name, Stack[0]->Uptime, Stack[0]->Frames, DeltaTime);
		Stack[0]->Uptime += DeltaTime;
		Stack[0]->Frames++;
	}
}

void UFSM::EnableTick()
{
	SetComponentTickEnabled(true);
}

void UFSM::DisableTick()
{
	SetComponentTickEnabled(false);
}

void UFSM::Start()
{
	#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString(GetName() + " has started"), true);
	#endif

	Stack[0]->OnEnterState.Broadcast();
	PreviousState = Stack[0];
	OnEnterAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
	AddStateToHistory(*Stack[0]);
	bIsRunning = true;

	EnableTick();
}

void UFSM::Stop()
{
	#if !UE_BUILD_SHIPPING
	if (bDebug)
		ULog::DebugMessage(SUCCESS, FString(GetName() + " has stopped"), true);
	#endif

	Stack[0]->OnExitState.Broadcast();
	PreviousState = Stack[0];
	OnExitAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
	AddStateToHistory(*Stack[0]);
	bIsRunning = false;

	DisableTick();
}

void UFSM::InitFSM(const int32 StateID)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("InitState: ") + GetName() + FString(" has already been initialized. This should be called once!"), true);
		#endif

		return;
	}

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			Stack.Add(&State);
			AddStateToHistory(State);

			bHasFSMInitialized = true;
			return;
		}
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
	#endif
}

void UFSM::InitFSM(const FName StateName)
{
	// Back out if we have already been initialized
	if (bHasFSMInitialized)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("InitState: FSM has already been initialized. This should be called once!"), true);
		#endif

		return;
	}

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			Stack.Add(&State);
			AddStateToHistory(State);

			bHasFSMInitialized = true;
			return;
		}
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("InitState: State does not exist"), true);
	#endif
}

void UFSM::AddState(const int32 ID, const FName StateName)
{
	for (FState& State : States)
	{
		if (State.ID == ID || State.Name == StateName)
		{
			#if !UE_BUILD_SHIPPING
			if (bDebug && bLogWarnings)
				ULog::DebugMessage(WARNING, FString("AddState: State '") + StateName.ToString() + FString("' already exists!"), true);
			#endif

			return;
		}
	}

	States.Add({ID, StateName});

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, FString("State " + StateName.ToString() + " added"), true);
	#endif
}

void UFSM::PopState()
{
	if (GetStateCountInStack() == 1)
		return;

	PopState(GetActiveStateID());
}

void UFSM::RemoveAllStates()
{
	for (int32 i = 0; i < Stack.Num(); i++)
		PopState();

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for one"), true);
	#endif
}

void UFSM::RemoveAllStatesExceptActive()
{
	for (int32 i = 0; i < Stack.Num(); i++)
	{
		if (Stack[i]->ID != GetActiveStateID())
			PopState();
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for the active state"), true);
	#endif
}

void UFSM::RemoveAllStatesExcept(const int32 ID)
{
	for (int32 i = 0; i < Stack.Num(); i++)
	{
		if (Stack[i]->ID != ID)
			PopState();
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for state ") + FString::FromInt(ID), true);
	#endif
}

void UFSM::RemoveAllStatesExcept(const FName StateName)
{
	for (int32 i = 0; i < Stack.Num(); i++)
	{
		if (Stack[i]->Name != StateName)
			PopState();
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogStatus)
		ULog::DebugMessage(INFO, FString("All states from the stack have been removed, except for state '") + StateName.ToString() + "'", true);
	#endif
}

void UFSM::PushState(const int32 StateID)
{
	if (GetActiveStateID() == StateID)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("PushState: State ID ") + FString::FromInt(StateID) + FString(" is already at the head of the stack."), true);
		#endif
		return;
	}

	for (FState& State : States)
	{
		if (State.ID == StateID)
		{
			PushState_Internal(State);

			return;
		}
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("PushState: State ID " + FString::FromInt(StateID) + " does not exist"), true);
	#endif
}

void UFSM::PushState(const FName StateName)
{
	if (GetActiveStateName() == StateName)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("PushState: State '") + StateName.ToString() + FString("' is already at the head of the stack."), true);
		#endif
		return;
	}

	for (FState& State : States)
	{
		if (State.Name == StateName)
		{
			PushState_Internal(State);

			return;
		}
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("PushState: State '" + StateName.ToString() + "' does not exist"), true);
	#endif
}

void UFSM::PushState_Internal(FState& State)
{
	Stack[0]->OnExitState.Broadcast();
	PreviousState = Stack[0];
	OnExitAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
	AddStateToHistory(*Stack[0]);
	Stack[0]->Uptime = 0;
	Stack[0]->Frames = 0;
	Stack.Insert(&State, 0);
	Stack[0]->OnEnterState.Broadcast();
	OnEnterAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
}

void UFSM::PopState_Internal(FState* State)
{
	Stack[0]->OnExitState.Broadcast();
	PreviousState = Stack[0];
	OnExitAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
	AddStateToHistory(*Stack[0]);
	Stack[0]->Uptime = 0;
	Stack[0]->Frames = 0;
	Stack.Remove(State);
	Stack[0]->OnEnterState.Broadcast();
	OnEnterAnyState.Broadcast(Stack[0]->ID, Stack[0]->Name);
}

void UFSM::AddStateToHistory(const FState& State)
{
	if (History.Num() < MaxHistoryStates)
	{
		History.Insert(State, 0);
	}
	else
	{
		History.RemoveAt(History.Num()-1);
		History.Shrink();
		History.Insert(State, 0);
	}
}

void UFSM::PopState(const int32 StateID)
{
	if (Stack.Num() == 1)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop state ") + FString::FromInt(StateID) + FString(" from the stack. At least one state should be in the stack!"), true);
		#endif
		return;
	}

	for (FState* State : Stack)
	{
		if (State->ID == StateID)
		{
			PopState_Internal(State);

			return;
		}
	}
}

void UFSM::PopState(const FName StateName)
{
	if (Stack.Num() == 1)
	{
		#if !UE_BUILD_SHIPPING
		if (bDebug && bLogWarnings)
			ULog::DebugMessage(WARNING, FString("PopState: Cannot pop '") + StateName.ToString() + FString("' state from the stack. At least one state should be in the stack!"), true);
		#endif

		return;
	}

	for (FState* State : Stack)
	{
		if (State->Name == StateName)
		{
			PopState_Internal(State);

			return;	
		}
	}
}

FState* UFSM::GetStateFromID(const int32 StateID)
{
	for (FState& State : States)
	{
		if (State.ID == StateID)
			return &State;
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("GetState: State '") + FString::FromInt(StateID) + FString("' does not exist"), true);
	#endif

	return nullptr;
}

FState* UFSM::GetStateFromName(const FName StateName)
{
	for (FState& State : States)
	{
		if (State.Name == StateName)
			return &State;
	}

	#if !UE_BUILD_SHIPPING
	if (bDebug && bLogWarnings)
		ULog::DebugMessage(WARNING, FString("GetState: State '") + StateName.ToString() + FString("' does not exist"), true);
	#endif

	return nullptr;
}

FName UFSM::GetStateName(const int32 ID) const
{
	for (const FState& State : States)
	{
		if (State.ID == ID)
			return State.Name;
	}

	#if !UE_BUILD_SHIPPING
	ULog::Error(FString("Failed to find the state name from ID. State ID ") + FString::FromInt(ID) + FString(" does not exist."), true);
	#endif

	return NAME_None;
}

int32 UFSM::GetStateID(const FName StateName) const
{
	for (const FState& State : States)
	{
		if (State.Name == StateName)
			return State.ID;
	}

	#if !UE_BUILD_SHIPPING
	ULog::Error(FString("Failed to find the state ID from name. State name '") + StateName.ToString() + FString("' does not exist."), true);
	#endif

	return -1;
}

FState* UFSM::GetActiveState() const
{
	return Stack[0];
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

int32 UFSM::GetPreviousStateFrames() const
{
	return PreviousState->Frames;
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
	for (const FState* State : Stack)
	{
		if (State->ID == StateID)
			return true;
	}

	return false;
}

bool UFSM::DoesStateExistInStack(const FName StateName)
{
	for (const FState* State : Stack)
	{
		if (State->Name == StateName)
			return true;
	}

	return false;
}

bool UFSM::DoesStateExist(const int32 StateID)
{
	for (const FState& State : States)
	{
		if (State.ID == StateID)
			return true;
	}

	return false;
}

bool UFSM::DoesStateExist(const FName StateName)
{
	for (const FState& State : States)
	{
		if (State.Name == StateName)
			return true;
	}

	return false;
}
