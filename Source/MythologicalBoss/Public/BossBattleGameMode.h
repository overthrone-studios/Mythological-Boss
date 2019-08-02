// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BossBattleGameMode.generated.h"

/**
 * The main game mode used for the boss battle
 */
UCLASS()
class MYTHOLOGICALBOSS_API ABossBattleGameMode final : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABossBattleGameMode();

protected:
	void BeginPlay() override;
};
