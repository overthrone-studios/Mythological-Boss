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
	FState(const int32 ID, const FName Name, FState* PreviousState = nullptr)
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

	FORCEINLINE TArray<FState> GetAllStates() const { return States; }
	FORCEINLINE int32 GetStatesCount() const { return States.Num(); }

	FState* GetState(int32 StateID);
	FState* GetState(const FName& StateName);

	FState* GetActiveState() const;
	int32 GetActiveStateID() const;
	FName GetActiveStateName() const;
	float GetActiveStateUptime() const;

	FState* GetPreviousState() const;
	int32 GetPreviousStateID() const;
	FName GetPreviousStateName() const;
	float GetPreviousStateUptime() const;

	void SetActiveState(int32 StateID);
	void SetActiveState(const FName& StateName);

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FState* ActiveState{};
	FState* PreviousState{};

	TArray<FState> States;
};
