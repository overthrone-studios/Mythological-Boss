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

	void InitState(int32 StateID);
	void InitState(const FName& StateName);

	void AddState(int32 ID, const FName& StateName);

	void PushState(int32 StateID);
	void PushState(const FName& StateName);
	void PopState(int32 StateID);
	void PopState(const FName& StateName);

	FORCEINLINE TArray<FState> GetAllStates() const { return States; }
	FORCEINLINE int32 GetStatesCount() const { return States.Num(); }

	FState* GetState(int32 StateID);
	FState* GetState(const FName& StateName);
	FState* GetStateInStack(int32 StateID);
	FState* GetStateInStack(const FName& StateName);

	FState* GetActiveState() const;
	int32 GetActiveStateID() const;
	FName GetActiveStateName() const;
	float GetActiveStateUptime() const;

protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FState*> Stack;

	TArray<FState> States;

	bool bHasFSMInitialized;
};
