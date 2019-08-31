// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockOn.generated.h"

UCLASS()
class MYTHOLOGICALBOSS_API ALockOn final : public AActor
{
	GENERATED_BODY()
	
public:	
	ALockOn();

protected:
	void BeginPlay() override;
};
