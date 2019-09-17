// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundBase.h"
#include "HitSoundData.generated.h"

/**
 * Used for playing a hit sound when a character has taken damage
 */
UCLASS()
class MYTHOLOGICALBOSS_API UHitSoundData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Hit Sound Data")
		TArray<USoundBase*> HitSounds;
};
