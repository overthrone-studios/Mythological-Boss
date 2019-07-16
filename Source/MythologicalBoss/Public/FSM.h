// Copyright Overthrone Studios 2019

#pragma once

#include "Components/ActorComponent.h"
#include "FSM.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHOLOGICALBOSS_API UFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSM();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
};
