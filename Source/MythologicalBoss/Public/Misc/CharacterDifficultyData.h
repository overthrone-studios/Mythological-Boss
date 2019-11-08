// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDifficultyData.generated.h"

/**
 * Data the character's will use based on the current difficulty selected
 */
UCLASS(Abstract)
class MYTHOLOGICALBOSS_API UCharacterDifficultyData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Difficulty Data Base | Health")
		float DefaultHealth = 100.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Difficulty Data Base | Combat")
		float DamageMultiplier = 1.0f;
};
