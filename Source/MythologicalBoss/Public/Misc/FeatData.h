// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FeatData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFeatAchievedSignature);

/**
 * The data containing a Feat's information
 */
UCLASS(BlueprintType)
class MYTHOLOGICALBOSS_API UFeatData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Feat Information")
		FText GetFeatProgress();

	// The name of the feat
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Feat Information")
		FText Title;
	
	// The description of the feat
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Feat Information")
		FText Description;

	// Does this feat have a progression?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Feat Information")
		uint8 bIsProgress : 1;

	// The maximum value for this progression feat to be considered complete
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Feat Information", meta = (EditCondition = "bIsProgress", ClampMin = 1))
		int32 MaxCount = 1;
	
	int32 CurrentCount = 0;

	UPROPERTY(BlueprintAssignable, Category = "Feat Information")
		FOnFeatAchievedSignature OnFeatAchieved;
};
