// Copyright Overthrone Studios 2019

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

	UPROPERTY(BlueprintReadOnly, Category = "State")
		int32 ID;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		int32 Frames;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		FName Name;

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

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void EnableTick();
	
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void DisableTick();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Start();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void InitFSM(int32 StateID);
		void InitFSM(FName StateName);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void AddState(int32 ID, FName StateName);

	void PopState();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesFromStack();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesFromStackExceptActive();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PushState(int32 StateID);
		void PushState(FName StateName);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PopState(int32 StateID);
		void PopState(FName StateName);

	FORCEINLINE TArray<FState> GetAllStates() const { return States; }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCount() const { return States.Num(); }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCountInStack() const { return Stack.Num(); }

	FState* GetStateFromID(int32 StateID);
	FState* GetStateFromName(FName StateName);

	FState* GetActiveState() const;
	FState* GetPreviousState() const;

	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetStateName(int32 ID) const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetStateID(FName StateName) const;

	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetPreviousStateID() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetPreviousStateName() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		float GetPreviousStateUptime() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetPreviousStateFrames() const;

	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetActiveStateID() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetActiveStateName() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		float GetActiveStateUptime() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetActiveStateFrames() const;

	UFUNCTION(BlueprintPure, Category = "FSM")
		bool DoesStateExistInStack(int32 StateID);
		bool DoesStateExistInStack(FName StateName);

	UFUNCTION(BlueprintPure, Category = "FSM")
		bool DoesStateExist(int32 StateID);
		bool DoesStateExist(FName StateName);

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsStackEmpty() const { return Stack.Num() == 0; }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsMachineRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE bool IsMachineInitialized() const { return bHasFSMInitialized; }

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

	TArray<FState*> Stack;

	TArray<FState> States;

	FState* PreviousState;

	uint8 bHasFSMInitialized : 1;
	uint8 bIsRunning : 1;
};
