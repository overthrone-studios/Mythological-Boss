// Copyright Overthrone Studios 2019

#pragma once

#include "Components/ActorComponent.h"
#include "FSM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateStateSignature);

USTRUCT()
struct FState
{
	GENERATED_USTRUCT_BODY()

	FState() : ID(0) {}
	FState(const int32 ID, const FName Name)
	{
		this->ID = ID;
		this->Name = Name;
	}

	UPROPERTY()
		FOnEnterStateSignature OnEnterState;
	UPROPERTY()
		FOnExitStateSignature OnExitState;
	UPROPERTY()
		FOnUpdateStateSignature OnUpdateState;

	int32 ID;
	FName Name;

	float Uptime = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHOLOGICALBOSS_API UFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSM();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Start();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void InitState(int32 StateID);
		void InitState(const FName& StateName);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void AddState(int32 ID, const FName& StateName);

	void PopState();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesFromStack();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesFromStackExceptActive();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PushState(int32 StateID);
		void PushState(const FName& StateName);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void PopState(int32 StateID);
		void PopState(const FName& StateName);

	FORCEINLINE TArray<FState> GetAllStates() const { return States; }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCount() const { return States.Num(); }

	UFUNCTION(BlueprintPure, Category = "FSM")
		FORCEINLINE int32 GetStateCountInStack() const { return Stack.Num(); }

	FState* GetState(int32 StateID);
	FState* GetState(const FName& StateName);
	FState* GetStateInStack(int32 StateID);
	FState* GetStateInStack(const FName& StateName);

	FState* GetActiveState() const;
	
	UFUNCTION(BlueprintPure, Category = "FSM")
		int32 GetActiveStateID() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		FName GetActiveStateName() const;
	UFUNCTION(BlueprintPure, Category = "FSM")
		float GetActiveStateUptime() const;

	UFUNCTION(BlueprintPure, Category = "FSM")
		bool DoesStateExistInStack(int32 StateID);
		bool DoesStateExistInStack(const FName& StateName);

	UFUNCTION(BlueprintPure, Category = "FSM")
		bool IsStackEmpty() const;

	// Enable debug mode?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FSM")
		uint8 bDebug : 1;

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FState*> Stack;

	TArray<FState> States;

	uint8 bHasFSMInitialized : 1;
	uint8 bIsRunning : 1;
};
