// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/Character.h"
#include "Mordath.generated.h"

UCLASS()
class MYTHOLOGICALBOSS_API AMordath final : public ACharacter
{
	GENERATED_BODY()

public:
	AMordath();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
};
