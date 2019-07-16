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

	FState() : ID(0), PreviousState(nullptr) {}
	FState(const int32 ID, const FName Name, FState* PreviousState = nullptr)
	{
		this->ID = ID;
		this->Name = Name;
		this->PreviousState = PreviousState;
	}

	UPROPERTY()
		FOnEnterStateSignature OnEnterState;
	UPROPERTY()
		FOnExitStateSignature OnExitState;
	UPROPERTY()
		FOnUpdateStateSignature OnUpdateState;

	int32 ID;
	FName Name;

	float RunningTime = 0;

	FState* PreviousState;
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

	FState* GetState(int32 StateID);
	FState* GetState(const FName& StateName);

	FState* GetActiveState() const;
	int32 GetActiveStateID() const;
	FName GetActiveStateName() const;

	float GetActiveStateRunningTime() const;

	void SetActiveState(int32 StateID);
	void SetActiveState(const FName& StateName);

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FState* ActiveState{};

	TArray<FState> States;
};
