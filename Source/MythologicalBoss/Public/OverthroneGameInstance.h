// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

};
