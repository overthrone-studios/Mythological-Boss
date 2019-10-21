// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "OverthroneCharacter.h"
#include "MordathBase.generated.h"

UCLASS()
class MYTHOLOGICALBOSS_API AMordathBase : public AOverthroneCharacter
{
	GENERATED_BODY()

public:
	AMordathBase();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
};
