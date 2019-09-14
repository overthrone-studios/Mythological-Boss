// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TutorialData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialCompletedSignature);

/**
 * Stores tutorial data for use in the tutorial section of the game
 */
UCLASS(BlueprintType)
class MYTHOLOGICALBOSS_API UTutorialData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Tutorial Information")
		FText GetTutorialProgress();

	// The message of the tutorial
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Tutorial Information")
		FText Message;

	// The amount of time (in seconds) we wait after completing this tutorial
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Tutorial Information")
		float Delay;

	// Does this tutorial have a progression?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Tutorial Information")
		uint8 bIsProgress : 1;

	// The maximum value for this progression tutorial to be considered complete
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Tutorial Information", meta = (EditCondition = "bIsProgress", ClampMin = 1))
		int32 MaxCount = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tutorial Information")
		int32 CurrentCount = 0;

	// Have we completed this tutorial?
	UPROPERTY(BlueprintReadOnly, Category = "Tutorial Information")
		uint8 bIsComplete : 1;

	UPROPERTY(BlueprintAssignable, Category = "Tutorial Information")
		FOnTutorialCompletedSignature OnTutorialCompleted;
};
