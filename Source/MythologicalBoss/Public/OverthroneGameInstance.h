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

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeMusicVolume(float SliderValue);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeSFXVolume(float SliderValue);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeUIVolume(float SliderValue);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeEnvironmentVolume(float SliderValue);

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float UIVolume = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float MusicVolume = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float SFXVolume = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		float EnvironmentVolume = 1.0f;

private:
	class USoundClass* MasterSoundClass;
	class USoundClass* MusicSoundClass;
	class USoundClass* SFXSoundClass;
	class USoundClass* UISoundClass;
	class USoundClass* EnvironmentSoundClass;

	class USoundMix* MasterMix;
};
