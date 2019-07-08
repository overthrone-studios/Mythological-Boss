// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

/**
 * High-level manager object for Overthrone game
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

	void OnStart() override;

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeMasterVolume(float SliderValue);

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;

private:
	class USoundClass* MasterSoundClass;
	class USoundClass* MusicSoundClass;
	class USoundClass* SFXSoundClass;
	class USoundClass* UISoundClass;
	class USoundClass* EnvironmentSoundClass;
	class USoundMix* MasterMix;
};
