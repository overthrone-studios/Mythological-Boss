// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "MordathDifficultyData.h"
#include "OverthroneGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamepadConnectionUpdatedSignature, bool, bIsConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoadedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameSavedSignature);

/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

	void Init() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Overthrone Game Instance")
		bool SaveGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Overthrone Game Instance")
		bool LoadGame();

	UPROPERTY(BlueprintAssignable, Category = "Gamepad")
		FOnGamepadConnectionUpdatedSignature OnGamepadConnectionUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Overthrone Game Instance")
		FOnGameLoadedSignature OnGameLoaded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Overthrone Game Instance")
		FOnGameSavedSignature OnGameSaved;

	UFUNCTION(BlueprintPure, Category = "Overthrone Game Instance")
		class UFeatData* GetFeat(const FString& FeatName);

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void ResetFeats();

	void InitFeats();
	void InitInstance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overthrone Game Instance")
		TArray<class UFeatData*> Feats;

	UFUNCTION()
		void OnFeatAchieved();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overthrone Game Instance")
		class UInputKeyToImageData* KeyboardKeysToTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overthrone Game Instance")
		class UInputKeyToImageData* GamepadKeysToTextures;

	class UFeatData* AchievedFeat;

	UFUNCTION()
		void OnControllerConnectionChanged(bool bIsConnected, int32 UserID, int32 UserIndex);

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Game Instance | Difficulty Option")
		TEnumAsByte<EDifficultyOptions> ChosenDifficultyOption;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 LightAttackUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 HeavyAttackUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 ChargeAttackUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 DashAttackUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 DashUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 BlockUses_GI = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Save Game | Player Actions")
		int32 ParryUses_GI = 0;

private:
	APlayerController* PlayerController{};
};
