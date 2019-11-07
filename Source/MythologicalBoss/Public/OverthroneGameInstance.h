// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamepadConnectionUpdatedSignature, bool, bIsConnected);

UENUM(BlueprintType)
enum EDifficultyOptions
{
	DO_Casual UMETA(DisplayName="Casual"),
	DO_Experienced UMETA(DisplayName="Experienced"),
	DO_Realistic UMETA(DisplayName="Realistic"),
	DO_None UMETA(DisplayName="None")
};

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

	UPROPERTY(BlueprintAssignable, Category = "Gamepad")
	FOnGamepadConnectionUpdatedSignature OnGamepadConnectionUpdated;

	UFUNCTION(BlueprintPure, Category = "Overthrone Game Instance")
		class UFeatData* GetFeat(const FString& FeatName);

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void ResetFeats();

	void InitFeats();
	void InitInstance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Instance")
		TArray<class UFeatData*> Feats;

	UFUNCTION()
		void OnFeatAchieved();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Instance")
		class UInputKeyToImageData* KeyboardKeysToTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Instance")
		class UInputKeyToImageData* GamepadKeysToTextures;

	class UFeatData* AchievedFeat;

	UFUNCTION()
		void OnControllerConnectionChanged(bool bIsConnected, int32 UserID, int32 UserIndex);

private:
	APlayerController* PlayerController{};
};
