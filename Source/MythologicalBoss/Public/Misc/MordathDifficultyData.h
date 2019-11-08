// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Misc/CharacterDifficultyData.h"
#include "MordathStageData.h"
#include "MordathDifficultyData.generated.h"

/**
 * Mordath's difficulty data
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathDifficultyData final : public UCharacterDifficultyData
{
	GENERATED_BODY()
	
public:
	UMordathDifficultyData();

	// The data the boss will use during stage 1 of the boss fight for this difficulty setting
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Difficulty Data | Combat")
		UMordathStageData* StageOneData;

	// The data the boss will use during stage 2 of the boss fight for this difficulty setting
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Difficulty Data | Combat")
		UMordathStageData* StageTwoData;

	// The data the boss will use during stage 3 of the boss fight for this difficulty setting
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mordath Difficulty Data | Combat")
		UMordathStageData* StageThreeData;
};
