// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OverthroneSaveGameBase.generated.h"

/**
 * Base class for an overthrone save game
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneSaveGameBase final : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 LightAttackUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 HeavyAttackUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 ChargeAttackUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 DashAttackUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 DashUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 BlockUses = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = "Overthrone Save Game | Player Actions")
		int32 ParryUses = 0;
};
