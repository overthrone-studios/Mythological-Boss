// Copyright Ali El Saleh 2019

#pragma once

#include "Components/ActorComponent.h"
#include "FSM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnterAnyStateSignature, int32, StateID, FName, StateName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExitAnyStateSignature, int32, StateID, FName, StateName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateAnyStateSignature, int32, StateID, FName, StateName, float, StateUptime, int32, StateFrames);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateStateSignature, float, StateUptime, int32, Frames);

USTRUCT(BlueprintType)
struct FState
{
	GENERATED_USTRUCT_BODY()

	FState() : ID(0), Frames(0) {}
	FState(const int32 ID, const FName Name)
	{
		this->ID = ID;
		this->Name = Name;
		Frames = 0;
	}

	UPROPERTY(BlueprintAssignable, Category = "State")
		FOnEnterStateSignature OnEnterState;
	UPROPERTY(BlueprintAssignable, Category = "State")
		FOnExitStateSignature OnExitState;
	UPROPERTY(BlueprintAssignable, Category = "State")
		FOnUpdateStateSignature OnUpdateState;

	// The state's ID
	UPROPERTY(BlueprintReadOnly, Category = "State")
		int32 ID;
	
	// The state's name
	UPROPERTY(BlueprintReadOnly, Category = "State")
		FName Name;

	// The state's current uptime in frames
	UPROPERTY(BlueprintReadOnly, Category = "State")
		int32 Frames;

	// The state's current uptime in seconds
	UPROPERTY(BlueprintReadOnly, Category = "State")
		float Uptime = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHOLOGICALBOSS_API UFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSM();

	UPROPERTY(BlueprintAssignable, Category = "FSM")
		FOnEnterAnyStateSignature OnEnterAnyState;

	UPROPERTY(BlueprintAssignable, Category = "FSM")
		FOnExitAnyStateSignature OnExitAnyState;
	
	UPROPERTY(BlueprintAssignable, Category = "FSM")
		FOnUpdateAnyStateSignature OnUpdateAnyState;

	// Enables this FSM component's tick function
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void EnableTick();
	
	// Disables this FSM component's tick function
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void DisableTick();

	// Starts the finite state machine
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Start();

	// Stops the finite state machine
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Stop();

	/**
	 * Initializes the finite state machine via the State ID
	 *
	 * @param  StateID - The state's unique ID
	*/
	void InitFSM(int32 StateID);

	/**
	 * Initializes the finite state machine via the State Name
	 *
	 * @param  StateName - The state's name
	*/
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void InitFSM(FName StateName);

	/**
	 * Adds a state to the States collection/array. Use this in the constructor to bind events to the returned state
	 *
	 * @param  ID - The state's ID
	 * @param  StateName - The state's name
	 *
	 * @return A reference to the newly added state
	*/
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void AddState(int32 ID, FName StateName);

	// Removes the state from the top of the stack (The current state)
	void PopState();

	// Removes all states from the stack, expect for the last state at the bottom of the stack
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStates();

	// Removes all states from the stack, expect for the active state
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesExceptActive();
	
	/**
	 * Removes all states from the stack, expect for the given state ID
	 *
	 * @param  ID - The state's ID
	*/
	void RemoveAllStatesExcept(int32 ID);

	/**
	 * Removes all states from the stack, expect for the given state name
	 *
	 * @param  StateName - The state's name
	*/
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesExcept(FName StateName);

	/**
	 * Pushes a state to the top of the stack, via the state's name
	 *
	 * @param  StateName - The state's name to search for and push
	*/
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PushState(FName StateName);

	/**
	 * Pushes a state to the top of the stack, via the state's ID
	 *
	 * @param  StateID - The state's ID to search for and push
	*/
	void PushState(int32 StateID);

	
	/**
	 * Removes the given state from the stack, using a name
	 *
	 * @param  StateName - The state's name to search for and remove
	*/
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PopState(FName StateName);

	/**
	 * Removes the given state from the stack, using an ID
	 *
	 * @param  StateID - The state's ID to search for and remove
	*/
	void PopState(int32 StateID);

	/**
	 * Retrieves all states that are stored in this state machine
	 *
	 * @return An array of states
	*/
	FORCEINLINE TArray<FState> GetAllStates() const { return States; }

	/**
	 * @return The number of states that are stored in this state machine
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCount() const { return States.Num(); }

	/**
	 * @return The number of states that are on the stack at this moment
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCountInStack() const { return Stack.Num(); }

	/**
	 * Retrieves a state using an ID
	 *
	 * @param  StateID - The state's ID
	 *
	 * @return A pointer to the retrieved state, if it exists. Otherwise null
	*/
	FState* GetStateFromID(int32 StateID);

	/**
	 * Retrieves a state using a name
	 *
	 * @param  StateName - The state's name
	 *
	 * @return A pointer to the retrieved state, if it exists. Otherwise null
	*/
	FState* GetStateFromName(FName StateName);

	/**
	 * Retrieves the active state from the stack
	 *
	 * @return A pointer to the active state
	*/
	FState* GetActiveState() const;
	
	/**
	 * Retrieves the previous state
	 *
	 * @return A pointer to the previous state
	*/
	FState* GetPreviousState() const;

	/**
	 * Retrieves the name of a state, given an ID
	 *
	 * @param  ID - The state's ID to search for
	 *
	 * @return The name of the state, if it exists. Otherwise NAME_None
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetStateName(int32 ID) const;

	/**
	 * Retrieves the ID of a state, given a name
	 *
	 * @param  StateName - The state's name to search for
	 *
	 * @return The ID of the state, if it exists. Otherwise -1
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetStateID(FName StateName) const;

	/**
	 * @return The previous state's ID
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetPreviousStateID() const;

	/**
	 * @return The previous state's name
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetPreviousStateName() const;

	/**
	 * @return The previous state's uptime
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		float GetPreviousStateUptime() const;

	/**
	 * @return The previous state's frames
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetPreviousStateFrames() const;

	/**
	 * @return The active state ID
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetActiveStateID() const;

	/**
	 * @return The active state name
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetActiveStateName() const;

	/**
	 * @return The active state uptime
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		float GetActiveStateUptime() const;

	/**
	 * @return The active state frames
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetActiveStateFrames() const;

	/**
	 * Test to see if a state exists in the stack, given a name
	 *
	 * @param  StateName - The state's name to search for
	 *
	 * @return True if state name exists in the stack. Otherwise false
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		bool DoesStateExistInStack(FName StateName);

	/**
	 * Test to see if a state exists in the stack, given an ID
	 *
	 * @param  StateID - The state's ID to search for
	 *
	 * @return True if state ID exists in the stack. Otherwise false
	*/
	bool DoesStateExistInStack(int32 StateID);

	/**
	 * Test to see if a state exists in this state machine, given a name
	 *
	 * @param  StateName - The state's name to search for
	 *
	 * @return True if state name exists in this state machine. Otherwise false
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		bool DoesStateExist(FName StateName);

	/**
	 * Test to see if a state exists in this state machine, given an ID
	 *
	 * @param  StateID - The state's ID to search for
	 *
	 * @return True if state ID exists in this state machine. Otherwise false
	*/
	bool DoesStateExist(int32 StateID);

	/**
	 * @return True if stack is empty
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsStackEmpty() const { return Stack.Num() == 0; }

	/**
	 * @return True if this state machine is currently running
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsMachineRunning() const { return bIsRunning; }

	/**
	 * @return True if this state machine has been initialized. To initialize this state machine, call InitState()
	*/
	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsMachineInitialized() const { return bHasFSMInitialized; }

	// The maximum amount of states that can be recored
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM", meta = (ClampMin = 1))
		int32 MaxHistoryStates = 50;

	// Toggle debug mode?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
		uint8 bDebug : 1;

	// Log the state machines status? (i.e what it's currently doing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM", meta = (EditCondition = "bDebug"))
		uint8 bLogStatus : 1;

	// Log state machine warnings?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM", meta = (EditCondition = "bDebug"))
		uint8 bLogWarnings : 1;

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PushState_Internal(FState& State);
	void PopState_Internal(FState* State);

	void AddStateToHistory(const FState& State);

	TArray<FState*> Stack;

	TArray<FState> States;

	TArray<FState> History;

	FState* PreviousState;

	uint8 bHasFSMInitialized : 1;
	uint8 bIsRunning : 1;
};
