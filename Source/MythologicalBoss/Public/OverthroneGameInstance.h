// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "Combat/MordathActionData.h"
#include "OverthroneEnums.h"
#include "OverthroneGameInstance.generated.h"


/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

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

	class UFeatData* AchievedFeat;

private:
	APlayerController* PlayerController{};
};
