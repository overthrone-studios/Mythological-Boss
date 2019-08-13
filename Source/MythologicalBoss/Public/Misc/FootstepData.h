// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundBase.h"
#include "FootstepData.generated.h"

/**
 * Stores an array of sounds and a pointer to a Physical Material
 */
UCLASS()
class MYTHOLOGICALBOSS_API UFootstepData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPhysicalMaterial* GetPhysicalMaterial() const {return PhysicalMaterial;}
	TArray<USoundBase*> GetFootstepSounds() const {return Sounds;}
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
		UPhysicalMaterial* PhysicalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
		TArray<USoundBase*> Sounds;
};
